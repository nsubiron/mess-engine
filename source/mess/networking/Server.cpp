#include "mess/networking/Server.h"

#include "mess/WeakPointerList.h"
#include "mess/networking/Session.h"

#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/spawn.hpp>
#include <boost/make_shared.hpp>

#include <iostream>

namespace mess {
namespace networking {

  using boost::asio::ip::tcp;

  Server::Server(boost::asio::io_service &io_service, const uint32_t port)
    : _io_service(io_service),
      _acceptor(io_service, tcp::endpoint(tcp::v4(), port)) {}

  void Server::Close()  {
    if (_acceptor.is_open())
      _acceptor.close();
  }

  void Server::Listen(CallbackFunction callback) {
    std::cout << "Starting server at " << _acceptor.local_endpoint() << "...\n";
    boost::asio::spawn(_io_service,
        [=](boost::asio::yield_context yield)
        {
          weak_ptr_list<Session> active_sessions;
          try {
            while (_acceptor.is_open()) {
              boost::system::error_code ec;
              tcp::socket socket(_io_service);
              _acceptor.async_accept(socket, yield[ec]);
              if (!ec) {
                auto ptr = boost::make_shared<Session>(std::move(socket));
                active_sessions.push_back(ptr);
                ptr->Go(callback);
              }
            }
          } catch (const std::exception &e) {
            std::cerr << "Server: Exception: " << e.what() << "\n";
          }
          active_sessions.for_each([](auto &session){ session.Close(); });
        });
  }

} // namespace networking
} // namespace mess
