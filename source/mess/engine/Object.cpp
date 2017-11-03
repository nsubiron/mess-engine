#include "mess/engine/Object.h"

#include "mess/engine/ObjectInitializer.h"

#include <atomic>

namespace mess {
namespace engine {

  static uint64_t GetUniqueId() {
    static std::atomic<uint64_t> COUNTER{0u};
    return COUNTER++;
  }

  Object::Object(const ObjectInitializer &oi)
    : _unique_id(GetUniqueId()),
      _owner_id(oi.GetPlayerId()),
      _world(oi.GetWorld()) {}

} // namespace engine
} // namespace mess
