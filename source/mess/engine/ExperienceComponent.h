#pragma once

#include "mess/engine/ActorComponent.h"
#include "mess/engine/Experience.h"

namespace mess {
namespace engine {

  class ExperienceComponent : public ActorComponent, public Experience {
    MESS_OBJECT_BODY(ExperienceComponent, ActorComponent)
  public:

    ExperienceComponent(const ObjectInitializer &oi, Actor &parent, uint64_t level);

    virtual void Tick(float delta_time) override;

  private:

    float _previous_level = 0;
  };

} // namespace engine
} // namespace mess
