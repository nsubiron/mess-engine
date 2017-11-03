#pragma once

#include <stdexcept>

namespace mess {
namespace engine {

  class access_denied_error : public std::invalid_argument {
  public:
    access_denied_error() : std::invalid_argument("access denied") {}
  };

} // namespace engine
} // namespace mess
