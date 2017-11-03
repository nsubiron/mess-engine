#pragma once

#include "mess/NonCopyable.h"
#include "mess/StopWatch.h"

#include <boost/asio/deadline_timer.hpp>
#include <boost/asio/io_service.hpp>

#include <atomic>

namespace mess {
namespace engine {

  class EngineTick;

  class Engine : private NonCopyable {
  public:

    explicit Engine(
        boost::asio::io_service &io_service,
        EngineTick &tick,
        boost::posix_time::milliseconds delta_time,
        float time_scale = 1.0f);

    bool done() const {
      return _done;
    }

    void set_done(bool done = true) {
      _done = done;
    }

    void AsyncLoop();

  private:

    void AsyncTick(const boost::system::error_code &err);

    boost::asio::io_service &_service;

    boost::asio::deadline_timer _deadline;

    std::atomic_bool _done;

    StopWatch _clock;

    const boost::posix_time::milliseconds _delta_time;

    const float _time_scale;

    EngineTick &_tick;
  };

} // namespace engine
} // namespace mess
