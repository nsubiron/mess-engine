#include "mess/engine/EngineTick.h"

#include "mess/engine/Tickable.h"

namespace mess {
namespace engine {

  EngineTick::EngineTick() {}

  EngineTick::~EngineTick() {}

  void EngineTick::BeginPlay() {
    std::lock_guard<std::mutex> lock(_mutex);
    DEBUG_ASSERT(!_is_running);
    _is_running = true;
    _tickables.for_each([](Tickable &tickable){
      tickable.BeginPlay();
    });
  }

  void EngineTick::Tick(float DeltaTime) {
    std::lock_guard<std::mutex> lock(_mutex);
    DEBUG_ASSERT(_is_running);
    _tick_list.for_each([DeltaTime](Tickable &tickable){
      if (tickable.IsTickEnabled()) {
        tickable.Tick(DeltaTime);
      }
    });
  }

  void EngineTick::EndPlay() {
    std::lock_guard<std::mutex> lock(_mutex);
    DEBUG_ASSERT(_is_running);
    _is_running = false;
    _tickables.for_each([](Tickable &tickable){
      tickable.EndPlay();
    });
  }

  void EngineTick::RegisterTickable(Tickable &tickable) {
    std::lock_guard<std::mutex> lock(_mutex);
    if (_is_running) {
      tickable.BeginPlay();
    }
    _tickables.push_back(tickable.WeakFromThis());
    if (tickable.CanEverTick()) {
      _tick_list.push_back(tickable.WeakFromThis());
    }
  }

  void EngineTick::DeregisterTickable(Tickable &tickable) {
    std::lock_guard<std::mutex> lock(_mutex);
    if (_is_running) {
      tickable.EndPlay();
    }
  }

} // namespace engine
} // namespace mess
