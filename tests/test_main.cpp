#include "utilities.h"
#include "gtest/gtest.h"

std::string port{};
std::string pseudo_port{};

int main(int argc, char **argv) {
  port = cobot::find_port();
  std::cout << "port = " << port << '\n';

  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
