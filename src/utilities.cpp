#include "myCobot.h"

#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <string_view>
#include <thread>

namespace cobot {

void cycle_colors(MyCobot &mc, unsigned int num_cycles) {
  for (decltype(num_cycles) i{}; i < num_cycles; ++i) {
    mc.set_color(Red{0xff}, Green{0x00}, Blue{0x00});
    std::this_thread::sleep_for(std::chrono::seconds(1));
    mc.set_color(Red{0x00}, Green{0xff}, Blue{0x00});
    std::this_thread::sleep_for(std::chrono::seconds(1));
    mc.set_color(Red{0x00}, Green{0x00}, Blue{0xff});
    std::this_thread::sleep_for(std::chrono::seconds(1));
  }
}

namespace macos {
auto find_port() -> std::string {
  std::string port;
  system(
      R"(ioreg -p IOUSB -l -b | grep -E "@|idVendor|idProduct|kUSBSerialNumberString|kUSBAddress" > .port_list)");

  std::ifstream port_list(".port_list");

  if (!port_list) {
    std::cerr << "Unable to open fileport_list.\n";
    std::exit(1);
  }
  std::string line;
  while (std::getline(port_list, line)) {
    if (std::string_view(line).contains("USB Single Serial") == true) {
      std::getline(port_list, line);
      auto idProduct = std::stoi(line.substr(line.rfind("= ") + 2));

      std::getline(port_list, line);
      auto kUSBSerialNumberString_raw = line.substr(line.rfind("= ") + 2);
      const auto first_quote = kUSBSerialNumberString_raw.find('\"');
      const auto last_quote = kUSBSerialNumberString_raw.rfind('\"');
      auto kUSBSerialNumberString = kUSBSerialNumberString_raw.substr(
          first_quote + 1, last_quote - first_quote - 1);

      std::getline(port_list, line);
      auto idVendor = std::stoi(line.substr(line.rfind("= ") + 2));

      std::getline(port_list, line);
      auto kUSBAddress = std::stoi(line.substr(line.rfind("= ") + 2));

      if (idProduct == 0x55d4 && idVendor == 0x1a86) {
        port = R"(/dev/cu.usbserial-)" + kUSBSerialNumberString +
               std::to_string(kUSBAddress);
        std::cout << "Found myCobot on serial port " << port << '\n';
      } else {
        std::cout << "myCobot not found\n";
      }

      break;
    }
  }
  port_list.close();
  return port;
}
} // namespace macos

} // namespace cobot