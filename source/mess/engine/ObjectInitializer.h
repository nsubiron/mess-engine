#pragma once

#include "mess/NonCopyable.h"
#include "mess/engine/PlayerId.h"

namespace mess {
namespace engine {

  class World;

  class ObjectInitializer : private NonCopyable {
  public:

    explicit ObjectInitializer(PlayerId player_id, World &world)
      : _player_id(player_id),
        _world(world) {}

    World &GetWorld() const {
      return _world;
    }

    PlayerId GetPlayerId() const {
      return _player_id;
    }

  private:

    const PlayerId _player_id;

    World &_world;
  };

} // namespace engine
} // namespace mess
