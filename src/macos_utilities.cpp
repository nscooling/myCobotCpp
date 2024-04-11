#include "macos_utilities.h"
#include "myCobot.h"

#include <array>
#include <cassert>
#include <cstdint>
#include <fstream>
#include <iostream>
// #include <print> // not suupported on gcc13
#include <sstream>
#include <string>
#include <string_view>
#include <thread>

namespace {

auto get_port_list(const char *system_call) -> std::string {
  std::array<char, 128> buffer;
  std::string result;
  auto pipe = popen(system_call, "r");
  if (!pipe) {
    std::cerr << "popen() failed!";
    std::exit(1);
  }
  while (fgets(buffer.data(), buffer.size(), pipe) != nullptr) {
    result += buffer.data();
  }
  [[maybe_unused]] auto returnCode = pclose(pipe);
  return result;
}

} // namespace

namespace cobot::macos {

constexpr auto sys_call =
    R"(ioreg -p IOUSB -l -b | grep -E "@|idVendor|idProduct|kUSBSerialNumberString|kUSBAddress")";

auto find_port() -> std::string {
  std::string port{};
  auto result = get_port_list(sys_call);
  std::istringstream port_list{result};
  std::string line;
  while (std::getline(port_list, line)) {
    if (std::string_view(line).contains("USB Single Serial")) {
      // print("(1) {}\n", std::string_view(line));
      std::getline(port_list, line);
      assert(std::string_view(line).contains("idProduct"));
      auto idProduct = std::stoi(line.substr(line.rfind("= ") + 2));
      // print("(2) {} {}\n", std::string_view(line), idProduct);

      std::getline(port_list, line);

      assert(std::string_view(line).contains("kUSBSerialNumberString"));
      auto kUSBSerialNumberString_raw = line.substr(line.rfind("= ") + 2);
      const auto first_quote = kUSBSerialNumberString_raw.find('\"');
      const auto last_quote = kUSBSerialNumberString_raw.rfind('\"');
      auto kUSBSerialNumberString = kUSBSerialNumberString_raw.substr(
          first_quote + 1, last_quote - first_quote - 1);
      // print("(3) {} {}\n", std::string_view(line), kUSBSerialNumberString);

      std::getline(port_list, line);
      assert(std::string_view(line).contains("idVendor"));
      auto idVendor = std::stoi(line.substr(line.rfind("= ") + 2));
      // print("(4) {} {}\n", std::string_view(line), idVendor);

      std::getline(port_list, line);
      int kUSBAddress{};

      // the string from ioreg is not consistent, so we need to check for
      // kUSBAddress for arm64 but not on x86_64
      // Probably should check for PortNum but this then screws up the
      // grep'ed string ordering.
      if (std::string_view(line).contains("kUSBAddress")) {
        kUSBAddress = std::stoi(line.substr(line.rfind("= ") + 2));
        // print("(5) {} {}\n", std::string_view(line), kUSBAddress);
      } else {
        // Kudge for x86_64 - assume it wil be port 1
        kUSBAddress = 1;
      }

      // if (idProduct == 21972 && idVendor == 6790) - decimal
      if (idProduct == 0x55d4 && idVendor == 0x1a86) {
        port = R"(/dev/cu.usbserial-)" + kUSBSerialNumberString + "1";
        std::to_string(kUSBAddress);
        break;
      }
    }
  }
  if (not port.empty()) {
    std::cout << "Found myCobot on serial port " << port << '\n';
  } else {
    std::cout << "myCobot not found\n";
    exit(EXIT_FAILURE);
  }
  return port;
}
} // namespace macos
