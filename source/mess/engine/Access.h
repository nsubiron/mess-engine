#pragma once

#include <cstdint>

namespace mess {
namespace engine {

  enum class Access : uint8_t {
    Everyone  = 0x1,
    Faction   = 0x2,
    Player    = 0x4,
    Master    = 0x8,
  };

} // namespace engine
} // namespace mess
