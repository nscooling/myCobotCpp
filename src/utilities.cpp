#include "myCobot.h"

#include <cstdint>
#include <iostream>
#include <thread>

namespace cobot {

// void print_angles(std::array<float, num_joints> angles) {
//   std::cout << "[ ";
//   for (auto a : angles) {
//     std::cout << a << " ";
//   }
//   std::cout << ']' << std::endl;
// }

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

} // namespace cobot