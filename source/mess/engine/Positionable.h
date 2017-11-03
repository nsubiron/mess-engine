#pragma once

#include "mess/engine/Math.h"

#include <mutex>

namespace mess {
namespace engine {

  class Positionable {
  public:

    Location GetLocation() const;

    void SetLocation(const Location &rhs);

    bool IsAtLocation(const Location &location) const;

    void MoveTowards(const Location &location, float distance);

  private:

    mutable std::mutex _mutex;

    Location _location;
  };

} // namespace engine
} // namespace mess
