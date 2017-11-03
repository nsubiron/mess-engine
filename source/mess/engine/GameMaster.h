#pragma once

#include "mess/NonCopyable.h"
#include "mess/engine/Player.h"
#include "mess/engine/PlayerId.h"
#include "mess/engine/Security.h"
#include "mess/engine/World.h"

#include <boost/shared_ptr.hpp>

#include <list>
#include <string>

namespace mess {
namespace engine {

  class GameMaster : private NonCopyable {
  public:

    explicit GameMaster(
        std::string game_name,
        const secure_string &game_password,
        const secure_string &master_password);

    template <typename T = Player>
    boost::shared_ptr<T> AddPlayer(
        const secure_string &game_password,
        const secure_string &player_password,
        std::string username) {
      const auto id = _auth.AddPlayer(game_password, player_password);
      auto player = _world.SpawnObject<T>(id, std::move(username));
      _players.push_back(player);
      return player;
    }

    World &GetWorld() {
      return _world;
    }

    EngineTick &GetEngineTick() {
      return _world.GetEngineTick();
    }

  private:

    std::string _game_name;

    Security _auth;

    PlayerId _master_id;

    std::list<boost::shared_ptr<Player>> _players;

    World _world;
  };

} // namespace engine
} // namespace mess
