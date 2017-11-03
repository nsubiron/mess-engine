#pragma once

#include "mess/engine/Object.h"

#include <atomic>

namespace mess {
namespace engine {

  class Tickable : public Object {
    MESS_OBJECT_BODY(Tickable, Object)
  public:

    Tickable(const ObjectInitializer &oi)
        : Super(oi),
          can_ever_tick(false),
          _is_tick_enabled(true) {}

    virtual void BeginPlay() {}

    virtual void Tick(float /*DeltaTime*/) {}

    virtual void EndPlay() {}

    bool CanEverTick() const {
      return can_ever_tick;
    }

    bool IsTickEnabled() const {
      return can_ever_tick && _is_tick_enabled;
    }

    void EnableTick(bool enable = true) {
      _is_tick_enabled = enable;
    }

  protected:

    std::atomic_bool can_ever_tick;

  private:

    std::atomic_bool _is_tick_enabled;
  };

} // namespace engine
} // namespace mess
