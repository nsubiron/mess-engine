#include "mess/engine/ActorComponent.h"

#include "mess/engine/Actor.h"

namespace mess {
namespace engine {

  ActorComponent::ActorComponent(const ObjectInitializer &oi, Actor &parent)
    : Super(oi),
      _parent(parent) {}

} // namespace engine
} // namespace mess
