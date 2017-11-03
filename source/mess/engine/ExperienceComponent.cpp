#include "mess/engine/ExperienceComponent.h"

#include "mess/engine/Actor.h"

namespace mess {
namespace engine {

  ExperienceComponent::ExperienceComponent(
      const ObjectInitializer &oi,
      Actor &parent,
      const uint64_t level)
    : Super(oi, parent),
      Experience(level),
      _previous_level(level) {
    can_ever_tick = true;
  }

  void ExperienceComponent::Tick(float delta_time) {
    Super::Tick(delta_time);
    const auto level = GetLevel();
    if (_previous_level < level) {
      logger().log(TR_ACTOR_LEVELED_UP, GetActor().GetName());
      _previous_level = level;
    }
  }

} // namespace engine
} // namespace mess
