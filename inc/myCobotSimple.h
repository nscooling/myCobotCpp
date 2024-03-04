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

struct Color {
  std::uint8_t r;
  std::uint8_t g;
  std::uint8_t b;
};

constexpr Color Red{0xff, 0x00, 0x00};
constexpr Color Green{0x00, 0xff, 0x00};
constexpr Color Blue{0x00, 0x00, 0xff};
constexpr Color Black{0x00, 0x00, 0x00};

class MyCobotSimple {
private:
  std::shared_ptr<Comms> serial;

public:
  explicit MyCobotSimple(std::string_view port);
  ~MyCobotSimple();

  MyCobotSimple(const MyCobotSimple &) = delete;
  MyCobotSimple &operator=(const MyCobotSimple &) = delete;

  // # Overall status
  //     version()
  auto get_basic_version() -> std::string;
  //     power_on()
  //     power_off()
  auto is_powered_on() -> bool;
  //     release_all_servos()
  auto is_controller_connected() -> bool;
  enum class refresh_mode_status { sequential, latest };
  auto get_fresh_mode() -> refresh_mode_status;

  auto start() -> void;
  auto stop() -> void;

  // # MDI mode and operation
  //     get_angles()
  auto get_angles() -> Angles;
  auto get_angle(Joint joint) -> float;

  // - **Prototype**: `send_angle(id, degree, speed)`
  // - **Description**: Send one degree of joint to robot arm.
  // - **Parameters**
  //   - `id`: Joint id(`genre.Angle`) / int 1-6
  //   - `degree`: degree value(`float`) (about -170 ～ 170)
  //   - `speed`: (`int`) 0 ~ 100
  auto send_angle(Joint joint, float angle, std::uint8_t speed = DefaultSpeed)
      -> void;

  // - **Prototype**: `send_angles(degrees, speed)`
  // - **Description**: Send the degrees of all joints to robot arm.
  // - **Parameters**
  //   - `degrees`: a list of degree value(`List[float]`), length 6.
  //   - `speed`: (`int`) 0 ~ 100
  auto send_angles(const Angles &angles, std::uint8_t speed = DefaultSpeed)
      -> void;

  //     get_coords()
  auto get_coords() -> Coords;
  //     send_coords()
  //     is_in_position()
  //     is_moving() x

  //     # JOG mode and operation
  // jog_angle()
  // jog_coord()
  // jog_stop()
  // set_encoder()
  // get_encoder()
  // set_encoders()
  // pause()
  // resume()
  // stop()
  // is_paused()

  // # Running status and Settings

  //   ### get_encoder(id, joint_id)
  // Obtain the specified joint potential value.
  // * **Parameters**
  //   * **id** – 1/2/3 (L/R/W).
  //   * **joint_id** – (int) 1 ~ 6
  // * **Returns**
  //     0 ~ 4096
  auto get_encoder(Joint joint) -> std::uint16_t;

  //     set_encoder()
  //     get_encoders()
  auto get_encoders() -> Encoders;
  //     set_encoders()
  //     get_speed()
  //     set_speed() *
  //     get_joint_min_angle()
  //     get_joint_max_angle()

  // # Servo control
  //     is_servo_enable()
  //     is_all_servo_enable()
  //     set_servo_data()
  //     get_servo_data()
  //     set_servo_calibration()
  //     release_servo()
  //     focus_servo()

  //     # Atom IO
  // set_color() *
  auto set_color(std::uint8_t r, std::uint8_t g, std::uint8_t b) -> void;
  auto set_color(Color color) -> void;
  // set_pin_mode()
  // set_digital_output()
  // get_digital_input()
  // set_pwm_mode() x
  // set_pwm_output()
  // get_gripper_value()
  // set_gripper_state() *
  // set_gripper_value()
  // set_gripper_ini()
  // is_gripper_moving()

  // # Basic
  //     set_basic_output()
  //     get_basic_input()

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
