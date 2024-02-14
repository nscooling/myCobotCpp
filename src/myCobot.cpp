#include "myCobot.h"

#include <charconv>
#include <cstdint>
#include <ranges>
#include <span>

#include "myCobotComms.h"

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

} // namespace

namespace cobot {

MyCobot::MyCobot(std::string_view port)
    : serial{std::make_shared<Comms>(port)} {}

MyCobot::~MyCobot() {
  stop();
  set_color(Red{0x00}, Green{0x00}, Blue{0x00});
  std::this_thread::sleep_for(std::chrono::seconds(2));
}

void MyCobot::set_color(Red r, Green g, Blue b) {
  serial->write(command::set_color, std::uint8_t(r), std::uint8_t(g),
                std::uint8_t(b));
}

bool MyCobot::is_powered_on() {
  auto read_buffer = serial->get(command::is_power_on);
  return (read_buffer[0] == 0x01);
}

auto MyCobot::get_angles() -> Angles {
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

auto MyCobot::set_angle(Joint joint, float angle, std::uint8_t speed) -> void {

  const auto joint_id = std::uint8_t(joint);
  const auto raw_angle = angle_to_i16(angle);
  const auto [high_byte, low_byte] = to_u8<std::int16_t>(raw_angle);

  serial->write(command::send_angle, joint_id, high_byte, low_byte, speed);
}

auto MyCobot::get_coords() -> Coords {
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

auto MyCobot::get_basic_version() -> std::string {
  auto read_buffer = serial->get(command::get_basic_version);
  assert(read_buffer.size() == 1);
  // std::ostringstream stream;
  // stream << read_buffer[0] / 10.0f;
  // return stream.str();
  std::array<char, 10> str;
  if (auto [ptr, ec] = std::to_chars(str.data(), str.data() + str.size(),
                                     read_buffer[0] / 10.0f);
      ec == std::errc())
    return std::string(str.data(), ptr);
  else
    return std::make_error_code(ec).message();
}

auto MyCobot::is_controller_connected() -> bool {
  auto read_buffer = serial->get(command::is_controller_connected);
  assert(read_buffer.size() == 1);
  return (read_buffer[0] == 0x01);
}

auto MyCobot::start() -> void { serial->write(command::resume); }
auto MyCobot::stop() -> void { serial->write(command::stop); }

#if 0
auto MyCobot::jog_angle(Joint joint_id, JogDirection direction, uint8_t speed)
    -> void {
  serial->write(command::jog_angle, std::uint8_t(joint_id),
                std::uint8_t(direction), speed);
}
#endif

auto MyCobot::get_encoders() -> Encoders {
  auto read_buffer = serial->get(command::get_encoders);
  assert(read_buffer.size() == 12);
  return chunk_u8<std::uint16_t>(read_buffer);
}

auto MyCobot::cJoint::set_angle(float angle, std::uint8_t speed) -> void {
  const auto raw_angle = angle_to_i16(angle);
  const auto [high_byte, low_byte] = to_u8<std::int16_t>(raw_angle);
  if (auto com = serial.lock()) {
    com->write(command::send_angle, joint, high_byte, low_byte, speed);
  }
}

auto MyCobot::cJoint::get_angle() -> float {
  if (auto com = serial.lock()) {
    auto read_buffer = com->get(command::get_angles);
    assert(read_buffer.size() == 12);
    const auto joint_offset = std::uint8_t((joint - 1) * 2);
    return i16_to_angle(from_u8<int16_t>(read_buffer[joint_offset],
                                         read_buffer[joint_offset + 1]));
  }
  return {};
}

auto MyCobot::cJoint::set_encoder(uint16_t value, std::uint8_t speed) -> void {
  const auto [high_byte, low_byte] = to_u8<std::uint16_t>(value);
  if (auto com = serial.lock()) {
    com->write(command::set_encoder, joint, high_byte, low_byte, speed);
  }
}

} // namespace cobot