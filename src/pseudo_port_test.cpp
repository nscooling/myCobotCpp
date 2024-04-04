#include "pseudo_port_test.h"
#include "myCobotComms.h"
#include "myCobotSimple.h"
#include "utilities.h"

#include <array>
#include <fstream>
#include <iostream>
#include <span>
#include <thread>

using namespace std::chrono_literals;

auto get_written_data(std::string const &pseudo_port, unsigned num)
    -> std::string {
  std::string command =
      "xxd -l " + std::to_string(num) + " -g 1 < " + pseudo_port;
  auto pipe = popen(command.c_str(), "r");
  if (!pipe) {
    std::cerr << "popen() failed!";
    std::exit(1);
  }
  std::array<char, 128> buffer;
  fgets(buffer.data(), buffer.size(), pipe);
  return buffer.data();
}

template <std::size_t N>
void send_to_cobot(std::string const &pseudo_port,
                   std::array<std::uint8_t, N> const &packet) {
  std::ofstream file(pseudo_port, std::ios::binary);
  if (!file) {
    std::cerr << "Cannot open file " << pseudo_port << '\n';
    std::exit(1);
    ;
  }
  std::cout << pseudo_port << " open; writing packet: ";
  cobot::print_byte_list(std::cout, std::span{packet});
  file.write(reinterpret_cast<const char *>(packet.data()), sizeof(packet));
  file.close();
}

void pseduo_port_test(std::string const &port, std::string const &pseudo_port) {

  {
    auto packet =
        cobot::make_frame(cobot::command::is_controller_connected, true);
    send_to_cobot(pseudo_port, packet);
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