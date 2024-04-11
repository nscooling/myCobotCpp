#include "myCobotSimple.h"
#include "gtest/gtest.h"
#include <string>

extern std::string port;

namespace { // use an unnamed namespace to ensure internal linkage.

class myCobotTests : public ::testing::Test {
protected:
  myCobotTests() = default;
  ~myCobotTests() override = default;
  // Objects declared here can be used by all tests in the test case
  cobot::MyCobotSimple mc{port};
};

} // namespace

TEST_F(myCobotTests, robotConnected) {
  ASSERT_TRUE(mc.is_controller_connected());
}

TEST_F(myCobotTests, robotVersion) {
  auto version = mc.get_basic_version();
  ASSERT_EQ(version, "2.4");
}

TEST_F(myCobotTests, robotRefreshMode) {
  auto refresh_mode = mc.get_refresh_mode();
  ASSERT_EQ(static_cast<int>(refresh_mode),
            int(cobot::MyCobotSimple::refresh_mode_status::latest));
}

TEST_F(myCobotTests, robotPowerStatus) { ASSERT_TRUE(mc.is_powered_on()); }

TEST_F(myCobotTests, servosAreEnabled) {
  ASSERT_TRUE(mc.is_servo_enabled(cobot::Joint::J6));
  ASSERT_TRUE(mc.is_servo_enabled(cobot::Joint::J5));
  ASSERT_TRUE(mc.is_servo_enabled(cobot::Joint::J4));
  ASSERT_TRUE(mc.is_servo_enabled(cobot::Joint::J3));
  ASSERT_TRUE(mc.is_servo_enabled(cobot::Joint::J2));
  ASSERT_TRUE(mc.is_servo_enabled(cobot::Joint::J1));
}

TEST_F(myCobotTests, allServosAreEnabled) {
  ASSERT_TRUE(mc.is_all_servo_enable());
}
