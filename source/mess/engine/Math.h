#pragma once

#include "mess/Debug.h"
#include "mess/NonCopyable.h"

#include <cmath>
#include <limits>

namespace mess {
namespace engine {
namespace math {

  using float_t = float;

  constexpr inline float_t epsilon() {
    return std::numeric_limits<float_t>::epsilon();
  }

  constexpr inline float_t infinity() {
    return std::numeric_limits<float_t>::infinity();
  }

  constexpr inline bool is_zero(float_t x, float_t scale = 1.0) {
    return std::abs(x) < scale * epsilon();
  }

  constexpr inline bool are_equal(float_t x, float_t y, float_t scale = 1.0) {
    return is_zero(x - y, scale);
  }

  constexpr inline bool is_less_equal(float_t x, float_t y, float_t scale = 1.0) {
    return x <= y + scale * epsilon();
  }

  constexpr inline bool is_in_range(float_t value, float_t minimum, float_t maximum, float_t scale = 1.0) {
    return
        (minimum - scale * epsilon() <= value)
        &&
        (value <= maximum + scale * epsilon());
  }

  class Vector2D {
  public:

    using value_type = float_t;

    float_t x;

    float_t y;

    constexpr float_t GetSquaredNorm() const {
      return x*x + y*y;
    }

    float_t GetLenght() const {
      return std::sqrt(GetSquaredNorm());
    }

    /// Throws runtime_error if cannot normalize.
    void Normalize();
  };

} // namespace math

  class Location : private math::Vector2D {
  public:

    using value_type = math::Vector2D::value_type;

    using math::Vector2D::x;
    using math::Vector2D::y;

    constexpr Location() : math::Vector2D{math::infinity(), math::infinity()} {}

    constexpr Location(value_type x, value_type y) : math::Vector2D{x, y} {}

    constexpr bool IsValid() const {
      return (x != math::infinity()) && (y != math::infinity());
    }
  };

  class Direction : private math::Vector2D {
  public:

    explicit Direction(const Location &start, const Location &end)
      : Vector2D{end.x - start.x, end.y - start.y} {
      Normalize();
    }

    float_t x() const {
      return math::Vector2D::x;
    }

    float_t y() const {
      return math::Vector2D::y;
    }
  };

  class Displacement : private math::Vector2D {
  public:

    using math::Vector2D::x;
    using math::Vector2D::y;

    constexpr explicit Displacement(const Location &start, const Location &end)
      : Vector2D{end.x - start.x, end.y - start.y} {}

    explicit Displacement(const Direction &direction, float_t distance)
      : Vector2D{direction.x() * distance, direction.y() * distance} {}

    constexpr value_type GetSquaredDistance() const {
      return GetSquaredNorm();
    }
  };

namespace math {

  /// Return true if @a lhs and @a rhs are separated by a distance strictly less
  /// than @a radius.
  inline bool intersect(const Location &rhs, const Location &lhs, float radius) {
    DEBUG_ASSERT(rhs.IsValid() && lhs.IsValid());
    return Displacement(rhs, lhs).GetSquaredDistance() < (radius*radius);
  }

  inline void move(Location &location, const Displacement &displacement) {
    location.x += displacement.x;
    location.y += displacement.y;
  }

  inline void move(Location &location, const Direction &direction, float distance) {
    math::move(location, Displacement(direction, distance));
  }

} // namespace math

} // namespace engine
} // namespace mess
