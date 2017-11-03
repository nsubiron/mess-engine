#include "mess/engine/Math.h"

#include <stdexcept>

namespace mess {
namespace engine {
namespace math {

  void Vector2D::Normalize() {
    const auto length = GetLenght();
    if (is_zero(length))
      throw std::runtime_error("cannot normalize vector of zero length");
    x /= length;
    y /= length;
    DEBUG_ASSERT(are_equal(1.0, GetLenght(), 2.0));
  }

} // namespace math
} // namespace engine
} // namespace mess
