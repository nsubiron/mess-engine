#pragma once

#include "mess/networking/Server.h"

#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/steady_timer.hpp>
#include <boost/asio/strand.hpp>
#include <boost/enable_shared_from_this.hpp>

#include <functional>
#include <mutex>
#include <string>

namespace mess {
namespace networking {

  class Session : public boost::enable_shared_from_this<Session> {
  public:

    explicit Session(boost::asio::ip::tcp::socket socket);

    ~Session();

    void Go(Server::CallbackFunction callback);

    void Close();

  private:

    mutable std::mutex _mutex;

    uint _id;

    boost::asio::ip::tcp::socket _socket;

    boost::asio::steady_timer _timer;

    boost::asio::io_service::strand _strand;
  };

} // namespace networking
} // namespace mess
