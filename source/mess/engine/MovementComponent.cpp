#include "mess/engine/MovementComponent.h"

#include "mess/engine/Actor.h"

#include <limits>

namespace mess {
namespace engine {

  MovementComponent::MovementComponent(const ObjectInitializer &oi, Actor &parent)
    : Super(oi, parent),
      _speed(0.0f),
      _goal() {
    can_ever_tick = true;
  }

  void MovementComponent::Tick(const float delta_time) {
    std::lock_guard<std::mutex> lock(_mutex);
    if (_goal.IsValid()) {
      GetActor().MoveTowards(_goal, delta_time * _speed);
    }
  }

  void MovementComponent::SetSpeed(const float speed) {
    std::lock_guard<std::mutex> lock(_mutex);
    _speed = std::abs(speed);
  }

  void MovementComponent::GoTo(const Location &goal) {
    std::lock_guard<std::mutex> lock(_mutex);
    _goal = goal;
  }

  bool MovementComponent::ReachedGoal() const {
    std::lock_guard<std::mutex> lock(_mutex);
    return !_goal.IsValid() || GetActor().IsAtLocation(_goal);
  }

} // namespace engine
} // namespace mess
