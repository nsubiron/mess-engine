#include "mess/networking/Session.h"

#include <boost/asio/read.hpp>
#include <boost/asio/spawn.hpp>
#include <boost/asio/write.hpp>

#include <atomic>
#include <iostream>

namespace mess {
namespace networking {

  static std::atomic<uint> COUNT{0u};

#define LOGD(m0, m1) std::cout << "Session" << _id << ": " << m0 << ' ' << m1 << std::endl;

  Session::Session(boost::asio::ip::tcp::socket socket) :
    _id(++COUNT),
    _socket(std::move(socket)),
    _timer(_socket.get_io_service()),
    _strand(_socket.get_io_service()) {
    LOGD("started.", "");
  }

  Session::~Session() {
    LOGD("closed.", "");
  }

  void Session::Close() {
    std::lock_guard<std::mutex> lock(_mutex);
    if (_socket.is_open())
      _socket.close();
  }

  void Session::Go(Server::CallbackFunction callback) {
    auto self(shared_from_this());
    boost::asio::spawn(_strand,
        [this, self, callback](boost::asio::yield_context yield) {
          try {
            for (;;) {
              _timer.expires_from_now(std::chrono::seconds(30));

              std::string message;
              {
                // Read message size.
                uint32_t size;
                boost::asio::async_read(
                    _socket,
                    boost::asio::buffer(&size, sizeof(decltype(size))),
                    yield);

                // Now read the actual message.
                auto buffer = std::make_unique<char[]>(size);
                boost::asio::async_read(
                    _socket,
                    boost::asio::buffer(buffer.get(), size),
                    yield);
                message.assign(buffer.get(), size);
              }
              // Do the callback.
              const std::string answer = callback(std::move(message));
              {
                // Prepend answer with its size.
                const uint32_t size = answer.size();
                constexpr uint32_t extra_size = sizeof(decltype(size));
                std::string full_answer;
                full_answer.reserve(size + extra_size);
                full_answer.assign(reinterpret_cast<const char*>(&size), extra_size);
                full_answer += answer;

                // Send answer.
                boost::asio::async_write(_socket, boost::asio::buffer(full_answer), yield);
              }
            }
          } catch (const std::exception &e) {
            LOGD("exception:", e.what());
            Close();
            _timer.cancel();
          }
        });

    boost::asio::spawn(_strand,
        [this, self](boost::asio::yield_context yield)
        {
          while (_socket.is_open()) {
            boost::system::error_code ignored_ec;
            _timer.async_wait(yield[ignored_ec]);
            if (_timer.expires_from_now() <= std::chrono::seconds(0)) {
              LOGD("warning:", "connection timed out");
              Close();
            }
          }
        });
  }

} // namespace networking
} // namespace mess
