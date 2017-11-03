#pragma once

#include "mess/engine/Object.h"
#include "mess/engine/PlayerCredentials.h"
#include "mess/engine/Resources.h"

namespace mess {
namespace engine {

  class Player : public Object {
    MESS_OBJECT_BODY(Player, Object)
  public:

    Player(const ObjectInitializer &oi, std::string username);

    PlayerId player_id() const {
      return _credentials.player_id();
    }

    std::string username() const {
      return _credentials.username();
    }

    Resources &GetResources() {
      return _resources;
    }

    const Resources &GetResources() const {
      return _resources;
    }

  private:

    PlayerCredentials _credentials;

    Resources _resources;
  };

} // namespace engine
} // namespace mess
