#ifndef _UTILITIES_H_
#define _UTILITIES_H_

#include <cstdint>
#include <iostream>
#include <span>

namespace cobot {

auto find_port() -> std::string;

template <typename T, std::size_t len>
void print_list(std::ostream &os, std::span<T, len> span) {
  os << "[ ";
  // for (auto a : span) {
  for (auto a = span.begin(); a != span.end(); ++a) {
    os << *a;
    if (a != std::prev(span.end())) {
      os << ", ";
    }
  }
  os << " ]" << std::endl;
}

template <typename T, std::size_t len>
void print_byte_list(std::ostream &os, std::span<T, len> span, char cr = '\n') {
  os << std::showbase << std::hex << "[ ";
  // for (auto a : span) {
  for (auto a = span.begin(); a != span.end(); ++a) {
    os << std::uint32_t(*a);
    if (a != std::prev(span.end())) {
      os << ", ";
    }
  }
  os << std::dec << " ]" << cr;
}

} // namespace cobot

#endif // _UTILITIES_H_
