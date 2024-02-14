#ifndef MYCOBOT_H
#define MYCOBOT_H

#include <array>
#include <cstdint>
#include <memory>
#include <string>

namespace cobot {

class Comms;

constexpr std::size_t num_joints = 6;
constexpr std::size_t num_axes = 6;

enum class Joint : std::uint8_t { J1 = 1, J2, J3, J4, J5, J6 };
enum class Axis : std::uint8_t { x = 1, y, z, rx, ry, rz };
using Coords = std::array<float, num_axes>;
using Angles = std::array<float, num_joints>;
using Encoders = std::array<std::uint16_t, num_joints>;

enum class Red : uint8_t {};
enum class Blue : uint8_t {};
enum class Green : uint8_t {};

constexpr const std::uint8_t DefaultSpeed = 30;

class MyCobot {
private:
  std::shared_ptr<Comms> serial;

public:
  explicit MyCobot(const char *port);
  ~MyCobot();

  MyCobot(const MyCobot &) = delete;
  MyCobot &operator=(const MyCobot &) = delete;

  auto is_powered_on() -> bool;
  auto set_color(Red r, Green g, Blue b) -> void;
  auto get_angles() -> Angles;
  auto set_angle(Joint joint, float angle, std::uint8_t speed = DefaultSpeed)
      -> void;
  auto get_coords() -> Coords;
  auto get_encoders() -> Encoders;
  auto get_basic_version() -> std::string;
  auto is_controller_connected() -> bool;
  auto start() -> void;
  auto stop() -> void;

  /*
  Jog control angle.
  * **Parameters**
    * **joint_id** – int 1-6.
    * **direction** – 0 - decrease, 1 - increase
    * **speed** – int (0 - 100)
  */
  enum class JogDirection : uint8_t { decrease = 0, increase = 1 };
  auto jog_angle(Joint joint_id, JogDirection direction, std::uint8_t speed)
      -> void;

  class cJoint {
  public:
    explicit cJoint(Joint j, std::weak_ptr<Comms> serial_)
        : joint{std::uint8_t(j)}, serial{serial_} {}
    auto set_angle(float angle, std::uint8_t speed = DefaultSpeed) -> void;
    auto set_encoder(uint16_t value, std::uint8_t speed = DefaultSpeed) -> void;
    auto get_angle() -> float;

  private:
    const std::uint8_t joint;
    std::weak_ptr<Comms> serial;
  };

  cJoint J1{Joint::J1, serial};
  cJoint J2{Joint::J2, serial};
  cJoint J3{Joint::J3, serial};
  cJoint J4{Joint::J4, serial};
  cJoint J5{Joint::J5, serial};
  cJoint J6{Joint::J6, serial};
};

} // namespace cobot

#endif // MYCOBOT_H
