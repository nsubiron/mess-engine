#pragma once

#include "mess/NonCopyable.h"
#include "mess/WeakPointerList.h"

#include <mutex>

namespace mess {
namespace engine {

  class Tickable;

  class EngineTick : private NonCopyable {
  public:

    EngineTick();

    ~EngineTick();

    void BeginPlay();

    void Tick(float DeltaTime);

    void EndPlay();

    void RegisterTickable(Tickable &tickable);

    void DeregisterTickable(Tickable &tickable);

  private:

    mutable std::mutex _mutex;

    bool _is_running = false;

    weak_ptr_list<Tickable> _tickables;

    weak_ptr_list<Tickable> _tick_list;
  };

} // namespace engine
} // namespace mess
