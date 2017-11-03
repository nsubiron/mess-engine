#pragma once

#include "mess/Debug.h"

#include <algorithm>
#include <cmath>
#include <cstdint>

namespace mess {
namespace engine {

  /// This basically sets the balance of the game.
  class Experience {
  public:

    Experience() = default;

    explicit Experience(uint64_t level) : _xp(level * level) {}

    uint64_t GetLevel() const {
      return std::sqrt(_xp); // C * sqrt(xp)
    }

    void AnnotateAttack() {
      _xp += 5.0;
    }

    void AnnotateKill() {
      _xp += 15.0;
    }

    void DoSomeWork(double delta_time) {
      DEBUG_ONLY(const auto previous_xp = _xp;)
      constexpr double factor = 1.0 / (4.0 * 3600.0);  // every 4 hours.
      _xp += factor * delta_time;
      DEBUG_ASSERT(_xp > previous_xp);
    }

    float GetProductionTime() const {
      // One hour minus 5min per level.
      return 3600.0f - 300.0f * std::min(static_cast<float>(GetLevel()), 50.0f);
    }

    float SalaryFactor() const {
      return 0.975f + 0.025f * static_cast<float>(GetLevel());
    }

  private:

    double _xp = 0.0f;
  };

} // namespace engine
} // namespace mess
