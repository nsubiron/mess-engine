#pragma once

#include "mess/engine/Tickable.h"

namespace mess {
namespace engine {

  class Actor;

  class ActorComponent : public Tickable {
    MESS_OBJECT_BODY(ActorComponent, Tickable)
  public:

    ActorComponent(const ObjectInitializer &oi, Actor &parent);

    Actor &GetActor() const {
      return _parent;
    }

  private:

    Actor &_parent;
  };

} // namespace engine
} // namespace mess
