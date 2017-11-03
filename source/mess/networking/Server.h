#pragma once

#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/tcp.hpp>

#include <functional>

namespace mess {
namespace networking {

  class Session;

  class Server {
  public:

    using CallbackFunction = std::function<std::string(std::string)>;

    explicit Server(boost::asio::io_service &io_service, uint32_t port);

    void Close();

    /// Start a server that listens to @a port.
    ///
    /// Every time a message is received @a callback is called. The return value
    /// of @a callback is sent back to the client.
    ///
    /// Every message is prepended by a uint32 indicating the size of the
    /// message. This is done on the fly and @a callback needs to do nothing
    /// about it. The client is expected to send messages this way.
    void Listen(CallbackFunction callback);

  private:

    boost::asio::io_service &_io_service;

    boost::asio::ip::tcp::acceptor _acceptor;
  };

} // namespace networking
} // namespace mess
