#ifndef MYCOBOTSimple_H
#define MYCOBOTSimple_H

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

constexpr const std::uint8_t DefaultSpeed = 30;

class MyCobotSimple {
private:
  std::shared_ptr<Comms> serial;

public:
  explicit MyCobotSimple(std::string_view port);
  ~MyCobotSimple();

  MyCobotSimple(const MyCobotSimple &) = delete;
  MyCobotSimple &operator=(const MyCobotSimple &) = delete;

  auto is_powered_on() -> bool;
  auto set_color(std::uint8_t r, std::uint8_t g, std::uint8_t b) -> void;
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
};

} // namespace cobot

#endif // MYCOBOTSimple_H
