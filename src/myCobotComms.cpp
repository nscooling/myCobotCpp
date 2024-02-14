#include "myCobotComms.h"

namespace cobot {

Comms::Comms(std::string_view port) : serial{io, std::string{port}} {
  serial.set_option(boost::asio::serial_port_base::baud_rate(115200));
}

auto Comms::read_(boost::asio::serial_port &serial) -> std::vector<uint8_t> {
  std::array<uint8_t, 4> header{};
  boost::asio::read(serial, boost::asio::buffer(header));
  assert(header[0] == frame_identity);
  assert(header[1] == frame_identity);

  std::vector<uint8_t> read_buffer(header[2] - 2);
  boost::asio::read(serial, boost::asio::buffer(read_buffer));

  std::array<uint8_t, 1> end_byte{};
  boost::asio::read(serial, boost::asio::buffer(end_byte));
  assert(end_byte[0] == end_frame);

  return read_buffer;
}

} // namespace cobot