#include "myCobotSimple.h"
#include "gtest/gtest.h"
#include <string>

extern std::string port;

namespace { // use an unnamed namespace to ensure internal linkage.

class myCobotTests : public ::testing::Test {
protected:
  myCobotTests() : mc(port) {}
  ~myCobotTests() override = default;
  // Objects declared here can be used by all tests in the test case
  cobot::MyCobotSimple mc;
};

} // namespace

TEST_F(myCobotTests, robotConnected) {
  ASSERT_TRUE(mc.is_controller_connected());
}

TEST_F(myCobotTests, robotVersion) {
  auto version = mc.get_basic_version();
  ASSERT_EQ(version, "2.4");
}
