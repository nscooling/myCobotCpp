#ifndef MYCOBOT_COMMS_H
#define MYCOBOT_COMMS_H

#include <array>
#include <cstdint>
#include <vector>

#include <utility>
// The appears to be a bug In the Ubuntu/GCC version of the boost library
// with this untility is required to fix the issue
#include <boost/asio.hpp>

#include "myCobotCommands.h"

namespace cobot {

constexpr std::uint8_t frame_identity = 0xfe;
constexpr std::uint8_t end_frame = 0xfa;

class Comms {
public:
  explicit Comms(std::string_view port);
  Comms(const Comms &) = delete;
  Comms &operator=(const Comms &) = delete;

  auto read() -> std::vector<uint8_t> { return read_(serial); }
  template <typename... Ts> auto write(command cmd, Ts... args) {
    return write_(serial, cmd, args...);
  }

  template <typename... Ts>
  auto get(command cmd, Ts... args) -> std::vector<uint8_t> {
    write(cmd, args...);
    return read();
  }

private:
  auto read_(boost::asio::serial_port &serial) -> std::vector<uint8_t>;

  template <typename... Ts> constexpr auto make_fame(command cmd, Ts... args) {
    constexpr std::size_t frame_size =
        sizeof...(Ts) + 2; // 2 => cmd and end_frame
    constexpr std::size_t packet_size =
        frame_size + 3; // 3 => frame_identity, frame_identity, frame_size
    return std::array<std::uint8_t, packet_size>{
        frame_identity,    frame_identity,        frame_size,
        std::uint8_t(cmd), std::uint8_t(args)..., end_frame};
  }

  template <typename... Ts>
  auto write_(boost::asio::serial_port &serial, command cmd, Ts... args) {
    auto frame = make_fame(cmd, args...);
    return boost::asio::write(serial, boost::asio::buffer(frame));
  }

  boost::asio::io_service io;
  boost::asio::serial_port serial;
};

} // namespace cobot

#endif // MYCOBOT_COMMS_H
