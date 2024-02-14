#include <array>
#include <boost/asio.hpp>
#include <cstdint>
#include <iostream>
#include <thread>

#include "myCobot.h"
#include "utilities.h"

using namespace std::chrono_literals;

using cobot::Blue;
using cobot::Green;
using cobot::Red;

int main(int argc, char *argv[]) {
  auto port = "/dev/tty.usbserial-56E30047721";
  if (argc == 2) {
    port = argv[1];
  } else if (argc > 2) {
    std::cerr << "Usage: " << argv[0] << " <serial port>\n";
    return 1;
  }

  cobot::MyCobot mc(port);

  auto print_all = [&]() {
    auto angles = mc.get_angles();
    std::cout << "get_angles = ";
    cobot::print_list(std::span{angles});

    auto coords = mc.get_coords();
    std::cout << "get_coords = ";
    cobot::print_list(std::span{coords});

    auto encoders = mc.get_encoders();
    std::cout << "get_encoders = ";
    cobot::print_list(std::span{encoders});
  };

  if (not mc.is_controller_connected()) {
    std::cerr << "Robot is not connected\n";
    exit(EXIT_FAILURE);
  }
  std::cout << "Robot is connected\n";

  if (auto atom_is_on = mc.is_powered_on()) {
    std::cout << "Atom is powered on\n";
  } else {
    std::cout << "Atom is powered off\n";
  }

#if 1
  // mc.set_angle(cobot::Joint::J5, 0.0f, 50);
  mc.J5.set_angle(90.0f, 50);
  std::this_thread::sleep_for(3s);
  print_all();
  auto j5_angle = mc.J5.get_angle();
  std::cout << "J5 angle = " << j5_angle << std::endl;
  mc.J5.set_angle(-90.0f, 50);
  std::this_thread::sleep_for(3s);
  print_all();
  j5_angle = mc.J5.get_angle();
  std::cout << "J5 angle = " << j5_angle << std::endl;
  mc.J5.set_angle(0.0f, 50);
  std::this_thread::sleep_for(3s);
  print_all();
  j5_angle = mc.J5.get_angle();
  std::cout << "J5 angle = " << j5_angle << std::endl;
  mc.J5.set_encoder(1028);
  std::this_thread::sleep_for(3s);
  print_all();
  j5_angle = mc.J5.get_angle();
  std::cout << "J5 angle = " << j5_angle << std::endl;
#endif

  cycle_colors(mc);

  auto version = mc.get_basic_version();
  std::cout << "get_basic_version = " << version << std::endl;

  std::this_thread::sleep_for(1s);
}
