#include <array>
#include <cstdint>
#include <iostream>
#include <thread>

#include <utility>
// The appears to be a bug In the Ubuntu/GCC version of the boost library
// with this untility is required to fix the issue
#include <boost/asio.hpp>

#if 0

// This has seperate classes for each joint
#include "myCobot.h"

using cobot::Blue;
using cobot::Green;

#else

// This has a single class for all joints
#include "myCobotSimple.h"
#endif

#include "utilities.h"

using namespace std::chrono_literals;
using cobot::Red;

////////////////////////////////////////////////////////////////////////////////

[[maybe_unused]] static void port_find_test();
[[maybe_unused]] static int status_tests(cobot::MyCobotSimple &mc);
[[maybe_unused]] static int all_joint_test(cobot::MyCobotSimple &mc);

////////////////////////////////////////////////////////////////////////////////
//
// Main
//
int main(int argc, char *argv[]) {
  std::string port;
  if (argc == 1) {
    port = cobot::macos::find_port();
  } else if (argc == 2) {
    port = argv[1];
  } else if (argc > 2) {
    std::cerr << "Usage: " << argv[0] << " [<serial port>]\n";
    return 1;
  }

  // cobot::MyCobot mc(port);
  cobot::MyCobotSimple mc(port);
  // main code
  if (not mc.is_controller_connected()) {
    std::cerr << "Robot is not connected\n";
    exit(EXIT_FAILURE);
  }
  std::cout << "Robot is connected\n";

  status_tests(mc);
  all_joint_test(mc);
}

////////////////////////////////////////////////////////////////////////////////
//
// Test J5
//
int status_tests(cobot::MyCobotSimple &mc) {
  if (mc.is_powered_on()) {
    std::cout << "Atom is powered on\n";
  } else {
    std::cout << "Atom is powered off\n";
  }

  auto version = mc.get_basic_version();
  std::cout << "get_basic_version = " << version << std::endl;

  auto fresh_mode = mc.get_fresh_mode();
  std::cout << "get_fresh_mode = " << static_cast<int>(fresh_mode) << std::endl;

  return 0;
}

////////////////////////////////////////////////////////////////////////////////
//
// Test J5
//
int all_joint_test(cobot::MyCobotSimple &mc) {

  // lambda functions
  [[maybe_unused]] auto print_all = [&]() {
    auto angles = mc.get_angles();
    std::cout << "get_angles   = ";
    cobot::print_list(std::span{angles});

    auto coords = mc.get_coords();
    std::cout << "get_coords   = ";
    cobot::print_list(std::span{coords});

    auto encoders = mc.get_encoders();
    std::cout << "get_encoders = ";
    cobot::print_list(std::span{encoders});
  };

  [[maybe_unused]] auto print_joint = [&](cobot::Joint joint) {
    std::cout << "J" << static_cast<int>(joint)
              << " angle = " << mc.get_angle(joint)
              << " encoder = " << mc.get_encoder(joint) << std::endl;
  };

  [[maybe_unused]] auto move_by_90 = [&](cobot::Joint joint, float angle) {
    std::cout << "setting J" << static_cast<int>(joint) << " to " << angle
              << "\n";
    mc.send_angle(joint, angle, 50);
    std::this_thread::sleep_for(3s);
    print_joint(joint);
    print_all();

    std::cout << "setting J6 to 0.0f\n";
    mc.send_angle(joint, 0.0f, 50);
    std::this_thread::sleep_for(3s);
    print_joint(joint);
    print_all();
  };

  mc.set_color(cobot::Green);
  print_all();

  constexpr cobot::Angles home{0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
  std::cout << "setting all to 0.0f\n";
  mc.send_angles(home, 50);
  std::this_thread::sleep_for(1s);
  print_all();

  move_by_90(cobot::Joint::J6, 90.0f);
  move_by_90(cobot::Joint::J5, 90.0f);
  move_by_90(cobot::Joint::J4, 90.0f);
  move_by_90(cobot::Joint::J3, 90.0f);
  move_by_90(cobot::Joint::J2, 90.0f);
  move_by_90(cobot::Joint::J1, 90.0f);

  mc.set_color(cobot::Black);
  mc.send_angles(home, 50); // mc.set_color(0, 0, 0);
  std::this_thread::sleep_for(3s);
  return 0;
}
// void test2(){
//   // // mc.set_angle(cobot::Joint::J5, 0.0f, 50);
//   // mc.J5.set_angle(90.0f, 50);
//   // std::this_thread::sleep_for(3s);
//   // // print_all();
//   // auto j5_angle = mc.J5.get_angle();
//   // std::cout << "J5 angle = " << j5_angle << std::endl;
//   // mc.J5.set_angle(-90.0f, 50);
//   // std::this_thread::sleep_for(3s);
//   // // print_all();
//   // j5_angle = mc.J5.get_angle();
//   // std::cout << "J5 angle = " << j5_angle << std::endl;
//   // mc.J5.set_angle(0.0f, 50);
//   // std::this_thread::sleep_for(3s);
//   // // print_all();
//   // j5_angle = mc.J5.get_angle();
//   // std::cout << "J5 angle = " << j5_angle << std::endl;

//   mc.J5.set_encoder(1024);
//   std::this_thread::sleep_for(3s);
//   // print_all();
//   auto j5_angle = mc.J5.get_angle();
//   std::cout << "J5 angle = " << j5_angle << std::endl;

//   mc.J5.set_encoder(2048);
//   std::this_thread::sleep_for(3s);
//   // print_all();
//   j5_angle = mc.J5.get_angle();
//   std::cout << "J5 angle = " << j5_angle << std::endl;

//   cycle_colors(mc);

//   mc.set_color(Red{0x00}, Green{0x00}, Blue{0x00});
// }

void port_find_test() {
  auto port = cobot::macos::find_port();
  std::cout << port << std::endl;
}
