#ifndef _UTILITIES_H_
#define _UTILITIES_H_

#include <iostream>
#include <span>

namespace cobot {

class MyCobot;

template <typename T, std::size_t len> void print_list(std::span<T, len> span) {
  std::cout << "[ ";
  // for (auto a : span) {
  for (auto a = span.begin(); a != span.end(); ++a) {
    std::cout << *a;
    if (a != std::prev(span.end())) {
      std::cout << ", ";
    }
  }
  std::cout << " ]" << std::endl;
}

template <typename T, std::size_t len>
void print_byte_list(std::ostream &os, std::span<T, len> span, char cr = '\n') {
  std::cout << std::showbase << std::hex << "[ ";
  // for (auto a : span) {
  for (auto a = span.begin(); a != span.end(); ++a) {
    os << std::uint32_t(*a);
    if (a != std::prev(span.end())) {
      os << ", ";
    }
  }
  os << std::dec << " ]" << cr;
}

// void cycle_colors(MyCobot &mc, unsigned int num_cycles = 1);

auto find_port() -> std::string;

} // namespace cobot

#endif // _UTILITIES_H_
