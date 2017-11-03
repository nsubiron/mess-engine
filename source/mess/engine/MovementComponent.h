#pragma once

#include "mess/engine/ActorComponent.h"
#include "mess/engine/Math.h"

#include <mutex>

namespace mess {
namespace engine {

  class MovementComponent : public ActorComponent {
    MESS_OBJECT_BODY(MovementComponent, ActorComponent)
  public:

    MovementComponent(const ObjectInitializer &oi, Actor &parent);

    virtual void Tick(float delta_time) override;

    void SetSpeed(float speed);

    void GoTo(const Location &goal);

    bool ReachedGoal() const;

  private:

    mutable std::mutex _mutex;

    float _speed;

    Location _goal;
  };

} // namespace engine
} // namespace mess
