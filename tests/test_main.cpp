#include "utilities.h"
#include "gtest/gtest.h"

std::string port{};

int main(int argc, char **argv) {
  port = cobot::find_port();
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
