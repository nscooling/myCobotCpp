#include <array>
#include <cstdint>
#include <iostream>
#include <thread>

#include <utility>
// The appears to be a bug In the Ubuntu/GCC version of the boost library
// with this untility is required to fix the issue
#include <boost/asio.hpp>

#if 0
#include "myCobot.h"

using cobot::Blue;
using cobot::Green;
#else
#include "myCobotSimple.h"
#endif

#include "utilities.h"

using namespace std::chrono_literals;
using cobot::Red;

[[maybe_unused]] static void test1(std::string_view port);

#include "myCobotComms.h"

int main(int argc, char *argv[]) {
  std::string port;
  if (argc == 1) {
    try {
      port = cobot::find_port();
    } catch (std::exception &ex) {
      std::cerr << ex.what() << std::endl;
      return (-1);
    }
  } else if (argc == 2) {
    port = argv[1];
  } else if (argc > 2) {
    std::cerr << "Usage: " << argv[0] << " [<serial port>]\n";
    return 1;
  }

  // test1(port);
}

void test1(std::string_view port) {
  cobot::MyCobotSimple mc(port);

  if (not mc.is_controller_connected()) {
    std::cerr << "Robot is not connected\n";
    exit(EXIT_FAILURE);
  }
  std::cout << "Robot is connected\n";

  cobot::Encoders encoders{};

  // mc.set_encoders(encoders);
  std::this_thread::sleep_for(3s);

  encoders = mc.get_encoders();
  std::cout << "get_encoders = ";
  cobot::print_list(std::span{encoders});
  std::this_thread::sleep_for(3s);
}

void test2(std::string_view port) {
  // cobot::MyCobot mc(port);
  cobot::MyCobotSimple mc(port);

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
              << " encoder = " << mc.get_encoder(cobot::Joint::J5) << std::endl;
  };

  if (not mc.is_controller_connected()) {
    std::cerr << "Robot is not connected\n";
    exit(EXIT_FAILURE);
  }
  std::cout << "Robot is connected\n";

  if (mc.is_powered_on()) {
    std::cout << "Atom is powered on\n";
  } else {
    std::cout << "Atom is powered off\n";
  }

  auto version = mc.get_basic_version();
  std::cout << "get_basic_version = " << version << std::endl;

  auto fresh_mode = mc.get_fresh_mode();
  std::cout << "get_fresh_mode = " << static_cast<int>(fresh_mode) << std::endl;

#if 0
  // // mc.set_angle(cobot::Joint::J5, 0.0f, 50);
  // mc.J5.set_angle(90.0f, 50);
  // std::this_thread::sleep_for(3s);
  // // print_all();
  // auto j5_angle = mc.J5.get_angle();
  // std::cout << "J5 angle = " << j5_angle << std::endl;
  // mc.J5.set_angle(-90.0f, 50);
  // std::this_thread::sleep_for(3s);
  // // print_all();
  // j5_angle = mc.J5.get_angle();
  // std::cout << "J5 angle = " << j5_angle << std::endl;
  // mc.J5.set_angle(0.0f, 50);
  // std::this_thread::sleep_for(3s);
  // // print_all();
  // j5_angle = mc.J5.get_angle();
  // std::cout << "J5 angle = " << j5_angle << std::endl;


  mc.J5.set_encoder(1024);
  std::this_thread::sleep_for(3s);
  // print_all();
  auto j5_angle = mc.J5.get_angle();
  std::cout << "J5 angle = " << j5_angle << std::endl;


  mc.J5.set_encoder(2048);
  std::this_thread::sleep_for(3s);
  // print_all();
  j5_angle = mc.J5.get_angle();
  std::cout << "J5 angle = " << j5_angle << std::endl;

  cycle_colors(mc);

  mc.set_color(Red{0x00}, Green{0x00}, Blue{0x00});

#else

  // mc.set_color(0, 0xff, 0);
  mc.set_color(cobot::Green);

  print_all();

  auto angles = mc.get_angles();

  std::cout << "setting J5 to 90.0f\n";
  // mc.send_angle(cobot::Joint::J5, 90.0f, 50);
  // angles[0] = 45.0f;
  // angles[1] = 10.0f;
  // angles[2] = 20.0f;
  // angles[3] = 20.0f;
  // angles[4] = 90.0f;
  // mc.send_angles(angles, 50);
  mc.send_angles(cobot::Angles{45.0f, 10.0f, 20.0f, 20.0f, 90.0f}, 50);
  std::this_thread::sleep_for(3s);
  // print_all();
  print_joint(cobot::Joint::J5);

  std::cout << "setting J5 to -90.0f\n";
  // mc.send_angle(cobot::Joint::J5, -90.0f, 50);
  angles[0] = -45.0f;
  angles[1] = -10.0f;
  angles[2] = -20.0f;
  angles[3] = -20.0f;
  angles[4] = -90.0f;
  mc.send_angles(angles, 50);
  std::this_thread::sleep_for(3s);
  // print_all();
  print_joint(cobot::Joint::J5);

  std::cout << "setting J5 to 0.0f\n";
  // mc.send_angle(cobot::Joint::J5, 0.0f, 50);
  angles[0] = 0.0f;
  angles[1] = 0.0f;
  angles[2] = 0.0f;
  angles[3] = 0.0f;
  angles[4] = 0.0f;
  mc.send_angles(angles, 50);
  std::this_thread::sleep_for(3s);
  // print_all();
  print_joint(cobot::Joint::J5);
  // mc.set_color(0, 0, 0);
  mc.set_color(cobot::Black);

#endif

  std::this_thread::sleep_for(1s);
}
