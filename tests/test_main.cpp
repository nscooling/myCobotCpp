#include "utilities.h"
#include "gtest/gtest.h"

std::string port{};
std::string pseudo_port{};

int main(int argc, char **argv) {
  if (argc == 1) {
    try {
      port = cobot::find_port();
    } catch (std::exception &ex) {
      std::cerr << ex.what() << std::endl;
      return (-1);
    }
  } else if (argc == 2) {
    port = argv[1];
  } else if (argc == 3) {
    port = argv[1];
    pseudo_port = argv[2];
  } else if (argc > 3) {
    std::cerr << "Usage: " << argv[0]
              << " [<serial port>] or [<pseudo serial port>] [<pseudo serial "
                 "port>]\n";
    return 1;
  }

  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
