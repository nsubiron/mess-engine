#include "mess/engine/World.h"

#include "mess/engine/Actor.h"

namespace mess {
namespace engine {

  void World::visit(Actor &actor) {
    _actors.push_back(actor.WeakFromThis());
  }

  void World::visit(Tickable &tickable) {
    _tick.RegisterTickable(tickable);
  }

} // namespace engine
} // namespace mess
