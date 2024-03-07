# TODO List

* make reading from serial non-blocking with timeout - something like this
```
#include <boost/asio.hpp>
#include <boost/bind.hpp>

boost::asio::io_service io;
boost::asio::serial_port port(io, "/dev/ttys026");

if (!port.is_open()) {
    std::cerr << "Cannot open port\n";
    return;
}

boost::asio::deadline_timer timer(io);
timer.expires_from_now(boost::posix_time::seconds(5)); // 5 seconds timeout

char byte;
boost::asio::async_read(port, boost::asio::buffer(&byte, 1),
    boost::bind(&boost::asio::deadline_timer::cancel, &timer));

timer.async_wait(boost::bind(&boost::asio::serial_port::cancel, &port));

io.run();
```