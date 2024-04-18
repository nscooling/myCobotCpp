#include "myCobotSimple.h"

#include <charconv>
#include <cstdint>
#include <iostream>
#include <ranges>
#include <span>

#include "myCobotComms.h"
#include "utilities.h"

// namespace ranges = std::ranges;
// namespace views = std::views;

namespace {

template <typename T>
concept Is16Bit =
    std::is_same_v<T, std::int16_t> || std::is_same_v<T, std::uint16_t>;

template <Is16Bit Ty>
constexpr auto from_u8(std::uint8_t msb, std::uint8_t lsb) -> Ty {
  return Ty((msb << 8) | lsb);
};

template <Is16Bit Ty> constexpr auto to_u8(Ty raw) {
  return std::make_tuple(std::uint8_t(raw >> 8), std::uint8_t(raw & 0xff));
};

constexpr int angle_scaling_factor = 100;
constexpr int coord_scaling_factor = 10;

auto i16_to_angle = [](std::int16_t raw) -> float {
  return float(raw) / angle_scaling_factor;
};
auto angle_to_i16 = [](float angle) -> std::int16_t {
  return std::int16_t(angle * angle_scaling_factor);
};
auto i16_to_coord = [](std::int16_t raw) -> float {
  return float(raw) / coord_scaling_factor;
};
[[maybe_unused]] auto coord_to_i16 = [](float angle) -> std::int16_t {
  return std::int16_t(angle * coord_scaling_factor);
};

template <Is16Bit Ty> auto chunk_u8(auto const &read_buffer) {
  std::array<Ty, cobot::num_joints> raw_angles{};
  for (auto rbuffer = std::begin(read_buffer); auto &a : raw_angles) {
    a = from_u8<Ty>(rbuffer[0], rbuffer[1]);
    rbuffer += 2;
  }
  return raw_angles;
};

auto angles_to_raw(const cobot::Angles &angles) {
  std::array<std::uint8_t, 12> raw_angles{};
  auto raw_iter = std::begin(raw_angles);
  for (auto angle : angles) {
    auto [high_byte, low_byte] = to_u8(angle_to_i16(angle));
    *raw_iter++ = high_byte;
    *raw_iter++ = low_byte;
  }
  return raw_angles;
}

auto encodes_to_u8(cobot::Encoders const &encoders) {
  std::array<std::uint8_t, 12> raw_encoders{};
  auto raw_iter = std::begin(raw_encoders);
  for (auto encoder : encoders) {
    auto [high_byte, low_byte] = to_u8(encoder);
    *raw_iter++ = high_byte;
    *raw_iter++ = low_byte;
  }
  return raw_encoders;
}

} // namespace

namespace cobot {

MyCobotSimple::MyCobotSimple(std::string_view port)
    : serial{std::make_shared<Comms>(port)} {}

MyCobotSimple::~MyCobotSimple() = default;

auto MyCobotSimple::get_refresh_mode() -> refresh_mode_status {
  auto read_buffer = serial->get(command::get_refresh_mode);
  assert(read_buffer.size() == 1);
  return (read_buffer[0] == 0x01) ? refresh_mode_status::latest
                                  : refresh_mode_status::sequential;
}

auto MyCobotSimple::set_color(std::uint8_t r, std::uint8_t g, std::uint8_t b)
    -> void {
  serial->write(command::set_color, r, g, b);
}
auto MyCobotSimple::set_color(Color color) -> void {
  set_color(color.r, color.g, color.b);
}

bool MyCobotSimple::is_powered_on() {
  auto read_buffer = serial->get(command::is_power_on);
  return (read_buffer[0] == 0x01);
}

auto MyCobotSimple::get_angles() -> Angles {
  auto read_buffer = serial->get(command::get_angles);
  assert(read_buffer.size() == 12);
  auto raw_to_angles = [](auto &read_buffer) {
    auto raw_angles = chunk_u8<std::int16_t>(read_buffer);

    // clang-format off
    // Clang libc++ 12.0.0 does not support std::views::chunk
    // auto transformed = read_buffer | std::views::chunk(2) |
    //                    std::views::transform([](auto pair) {
    //                      return static_cast<int16_t>(pair[0] << 8 | pair[1]);
    //                    });
    // std::ranges::copy(transformed, std::begin(raw_angles));
    // clang-format on

    Angles angles{};
    std::ranges::transform(raw_angles, std::begin(angles), i16_to_angle);
    // auto r = raw_angles |
    //          std::views::transform([](auto raw) { return raw / 100.0f; });
    // std::ranges::copy(r, std::begin(angles));

    return angles;
  };

  return raw_to_angles(read_buffer);
}

auto MyCobotSimple::get_angle(Joint joint) -> float {
  auto read_buffer = serial->get(command::get_angles);
  assert(read_buffer.size() == 12);
  const auto joint_offset = std::uint8_t((std::uint8_t(joint) - 1) * 2);
  return i16_to_angle(from_u8<int16_t>(read_buffer[joint_offset],
                                       read_buffer[joint_offset + 1]));
}

auto MyCobotSimple::send_angle(Joint joint, float angle, std::uint8_t speed)
    -> void {

  const auto joint_id = std::uint8_t(joint);
  const auto raw_angle = angle_to_i16(angle);
  const auto [high_byte, low_byte] = to_u8<std::int16_t>(raw_angle);

  serial->write(command::send_angle, joint_id, high_byte, low_byte, speed);
}

auto MyCobotSimple::send_angles(const Angles &angles, std::uint8_t speed)
    -> void {
  auto [p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11] =
      angles_to_raw(angles);
  serial->write(command::send_angles, p0, p1, p2, p3, p4, p5, p6, p7, p8, p9,
                p10, p11, speed);
}

auto MyCobotSimple::get_coords() -> Coords {
  auto read_buffer = serial->get(command::get_coords);
  assert(read_buffer.size() == 12);

  auto raw_coords = chunk_u8<std::int16_t>(read_buffer);
  Coords coords{};
  std::transform(std::begin(raw_coords), std::end(raw_coords),
                 std::begin(coords), i16_to_coord);
  std::transform(std::begin(raw_coords) + 3, std::end(raw_coords),
                 std::begin(coords) + 3, i16_to_angle);
  return coords;
}

auto MyCobotSimple::get_basic_version() -> std::string {
  auto read_buffer = serial->get(command::get_basic_version);
  assert(read_buffer.size() == 1);
  std::array<char, 10> str;
  if (auto [ptr, ec] = std::to_chars(str.data(), str.data() + str.size(),
                                     read_buffer[0] / 10.0f);
      ec == std::errc())
    return std::string(str.data(), ptr);
  else
    return std::make_error_code(ec).message();
}

auto MyCobotSimple::is_controller_connected() -> bool {
  auto read_buffer = serial->get(command::is_controller_connected);
  assert(read_buffer.size() == 1);
  return (read_buffer[0] == 0x01);
}

auto MyCobotSimple::start() -> void { serial->write(command::resume); }
auto MyCobotSimple::stop() -> void { serial->write(command::stop); }

#if 0
auto MyCobotSimple::jog_angle(Joint joint_id, JogDirection direction, uint8_t speed)
    -> void {
  serial->write(command::jog_angle, std::uint8_t(joint_id),
                std::uint8_t(direction), speed);
}
#endif

auto MyCobotSimple::get_encoders() -> Encoders {
  auto read_buffer = serial->get(command::get_encoders);
  assert(read_buffer.size() == 12);
  return chunk_u8<std::uint16_t>(read_buffer);
}

auto MyCobotSimple::get_encoder(Joint joint) -> std::uint16_t {
  auto read_buffer = serial->get(command::get_encoder, joint);
  assert(read_buffer.size() == 2);
  return from_u8<std::uint16_t>(read_buffer[0], read_buffer[1]);
}

auto MyCobotSimple::release_all_servos() -> void {
  serial->write(command::release_all_servos);
}

auto MyCobotSimple::is_servo_enabled(Joint joint) -> bool {
  auto read_buffer = serial->get(command::is_servo_enable, joint);
  // cobot::print_byte_list(std::cout, std::span{read_buffer});
  assert(read_buffer.size() == 2);
  assert(read_buffer[0] == std::uint8_t(joint));
  constexpr std::uint8_t servo_enabled = 0x01;
  return (read_buffer[1] == servo_enabled);
}

auto MyCobotSimple::is_all_servo_enable() -> bool {
  auto read_buffer = serial->get(command::is_all_servo_enable);
  assert(read_buffer.size() == 1);
  return (read_buffer[0] == 0x01);
}

auto MyCobotSimple::set_encoder(Joint joint, std::uint16_t encoder) -> void {
  auto [high_byte, low_byte] = to_u8(encoder);
  serial->write(command::set_encoder, std::uint8_t(joint), high_byte, low_byte);
}

auto MyCobotSimple::set_encoders(Encoders const &encoders, std::uint8_t speed)
    -> void {
  auto [p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11] =
      encodes_to_u8(encoders);
  serial->write(command::set_encoders, p0, p1, p2, p3, p4, p5, p6, p7, p8, p9,
                p10, p11, speed);
}

auto MyCobotSimple::set_servo_calibration(Joint joint) -> void {
  serial->write(command::set_servo_calibration, std::uint8_t(joint));
}

} // namespace cobot