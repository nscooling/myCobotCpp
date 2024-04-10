#include "linux_utilities.h"

#include <cassert>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <string_view>
#include <thread>

namespace cobot {

namespace ubuntu {
auto find_port() -> std::string {
  std::string port;
  system(R"( hwinfo --usb > .port_list)");

  std::ifstream port_list(".port_list");

  if (!port_list) {
    std::cerr << "Unable to open fileport_list.\n";
    std::exit(1);
  }
  std::string line;
  while (std::getline(port_list, line)) {
    if (std::string_view(line).contains("Modem")) {
      do {
        std::getline(port_list, line);
      } while (not std::string_view(line).contains("Vendor"));
      if (std::string_view(line).contains("0x1a86")) {
        std::getline(port_list, line);
        assert(std::string_view(line).contains("Device"));
        assert(std::string_view(line).contains("0x55d4"));
        do {
          std::getline(port_list, line);
        } while (not std::string_view(line).contains("Device File"));
        port = line.substr(line.rfind("/dev/"));
        // std::cout << "Found port " << port << "\n";
      } else {
        continue;
      }
    }
  }
  return port;
}
} // namespace ubuntu

} // namespace cobot