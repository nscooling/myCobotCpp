#include <array>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <thread>
// #include <print>
// #include <fstream>

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

#include "myCobotComms.h"
#include "pseudo_port_test.h"
#include "utilities.h"

using namespace std::chrono_literals;
using cobot::Red;

////////////////////////////////////////////////////////////////////////////////

[[maybe_unused]] static int all_joint_test(cobot::MyCobotSimple &mc);

#include "myCobotComms.h"

////////////////////////////////////////////////////////////////////////////////
//
// Main
//
int main(int argc, char *argv[]) {
  std::string port{};
  std::string pseudo_port{};
  if (argc == 1) {
    try {
      port = cobot::find_port();
    } catch (std::exception &ex) {
      std::cerr << ex.what() << std::endl;
      return (-1);
    }
  } else if (argc == 2) {
    port = argv[1];
  } else if (argc == 3) {
    port = argv[1];
    pseudo_port = argv[2];
  } else if (argc > 3) {
    std::cerr << "Usage: " << argv[0]
              << " [<serial port>] or [<pseudo serial port>] [<pseudo serial "
                 "port>]\n";
    return 1;
  }

  if (pseudo_port.empty()) {
    std::this_thread::sleep_for(500ms);
    std::cout << "connecting to cobot\n";

    cobot::MyCobotSimple mc(port);
    if (not mc.is_controller_connected()) {
      std::cerr << "Robot is not connected\n";
      exit(EXIT_FAILURE);
    }
    std::cout << "Robot is connected\n";
    all_joint_test(mc);
  } else {
    pseduo_port_test(port, pseudo_port);
  }
  return 0;
}

////////////////////////////////////////////////////////////////////////////////
//
// Test Movement of all Joints by 90deg
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

    std::cout << "setting J" << static_cast<int>(joint) << " to 0.0f\n";
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
