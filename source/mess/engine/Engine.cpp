#include "mess/engine/Engine.h"

#include "mess/engine/EngineTick.h"

#include <iostream>

namespace mess {
namespace engine {

  Engine::Engine(
      boost::asio::io_service &io_service,
      EngineTick &tick,
      boost::posix_time::milliseconds delta_time,
      const float time_scale)
    : _service(io_service),
      _deadline(_service),
      _done(false),
      _delta_time(delta_time),
      _time_scale(1e-6 * time_scale),
      _tick(tick) {}

  void Engine::AsyncLoop() {
    _service.post([&](){ _tick.BeginPlay(); });
    _clock.Restart();
    _deadline.expires_from_now(_delta_time);
    _deadline.async_wait([this](const boost::system::error_code &err){ AsyncTick(err); });
  }

  void Engine::AsyncTick(const boost::system::error_code &err) {
    if (err) {
      std::cerr << "async tick: system error: " << err.message() << '\n';
      return;
    } else if (_done) {
      _tick.EndPlay();
      return;
    }

    const float seconds = _time_scale * _clock.GetElapsedTime<std::chrono::microseconds>();
    _clock.Restart();

    _tick.Tick(seconds);

    _deadline.expires_at(_deadline.expires_at() + _delta_time);
    _deadline.async_wait([this](const boost::system::error_code &err){ AsyncTick(err); });
  }

} // namespace engine
} // namespace mess
