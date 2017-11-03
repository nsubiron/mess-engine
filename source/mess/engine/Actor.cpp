#include "mess/engine/Actor.h"

namespace mess {
namespace engine {

  Actor::Actor(const ObjectInitializer &oi)
    : Actor(oi, "unnamed actor") {}

  Actor::Actor(const ObjectInitializer &oi, std::string name)
    : Actor(oi, std::move(name), "unknown") {}

  Actor::Actor(const ObjectInitializer &oi, std::string name, std::string type)
    : Super(oi),
      _name(std::move(name)),
      _type(std::move(type)) {}

  Actor::~Actor() {
    logger().debug("Actor '%s' destroyed", _name);
  }

} // namespace engine
} // namespace mess
