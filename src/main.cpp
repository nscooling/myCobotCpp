#include <array>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <thread>
// #include <print>
#include <fstream>

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

#include "myCobotComms.h"
#include "utilities.h"

using namespace std::chrono_literals;
using cobot::Red;

[[maybe_unused]] static void test1(std::string_view port);

#include "myCobotComms.h"

auto get_written_data(std::string const &pseudo_port, unsigned num)
    -> std::string {
  std::string command =
      "xxd -l " + std::to_string(num) + " -g 1 < " + pseudo_port;
  auto pipe = popen(command.c_str(), "r");
  if (!pipe) {
    std::cerr << "popen() failed!";
    std::exit(1);
  }
  // std::cout << "popen() succeeded\n";
  std::array<char, 128> buffer;
  // if(fgets(buffer.data(), buffer.size(), pipe) != nullptr) {
  //   std::cout << buffer.data() << '\n';
  // }
  fgets(buffer.data(), buffer.size(), pipe);
  return buffer.data();
}

template <std::size_t N>
void send_to_cobot(std::string const &pseudo_port,
                   std::array<std::uint8_t, N> const &packet) {
  std::ofstream file(pseudo_port, std::ios::binary);
  if (!file) {
    std::cerr << "Cannot open file " << pseudo_port << '\n';
    std::exit(1);;
  }
  std::cout << pseudo_port << " open; writing packet: ";
  cobot::print_byte_list(std::cout, std::span{packet});
  file.write(reinterpret_cast<const char *>(packet.data()), sizeof(packet));
  file.close();
}

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

  {
    // std::ofstream file(pseudo_port, std::ios::binary);
    // if (!file) {
    //   std::cerr << "Cannot open file " << pseudo_port << '\n';
    //   return -1;
    // }
    // std::cout << pseudo_port << " open for writing\n";
    auto packet =
        cobot::make_frame(cobot::command::is_controller_connected, true);
    send_to_cobot(pseudo_port, packet);
    // cobot::print_byte_list(std::cout, std::span{packet}
    // );
    // file.write(reinterpret_cast<const char*>(packet.data()), sizeof(packet));
    // file.close();
  }

  std::this_thread::sleep_for(500ms);
  std::cout << "connecting to cobot\n";

  cobot::MyCobotSimple mc(port);
  if (not mc.is_controller_connected()) {
    std::cerr << "Robot is not connected\n";
    exit(EXIT_FAILURE);
  }
  std::cout << "Robot is connected\n";

  auto data = get_written_data(pseudo_port, 5);
  std::cout << "data = " << data << '\n';
}

////////////////////////////////////////////////////////////

[[maybe_unused]] static void test1(std::string_view port) {
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

[[maybe_unused]] static void test2(std::string_view port) {
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
