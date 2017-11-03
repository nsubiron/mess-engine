#include "mess/engine/Positionable.h"

namespace mess {
namespace engine {

  static bool CloseEnough(const Location &location, const Location &goal) {
    constexpr float margin = 0.5f;
    return !goal.IsValid() || math::intersect(location, goal, margin);
  }

  Location Positionable::GetLocation() const {
    std::lock_guard<std::mutex> lock(_mutex);
    return _location;
  }

  void Positionable::SetLocation(const Location &rhs) {
    std::lock_guard<std::mutex> lock(_mutex);
    _location = rhs;
  }

  bool Positionable::IsAtLocation(const Location &location) const {
    std::lock_guard<std::mutex> lock(_mutex);
    return CloseEnough(_location, location);
  }

  void Positionable::MoveTowards(const Location &location, const float distance) {
    std::lock_guard<std::mutex> lock(_mutex);
    if (!CloseEnough(_location, location)) {
      Direction direction(_location, location);
      math::move(_location, direction, distance);
    }
  }

} // namespace engine
} // namespace mess
