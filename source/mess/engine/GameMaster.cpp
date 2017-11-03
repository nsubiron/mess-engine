#include "mess/engine/GameMaster.h"

namespace mess {
namespace engine {

  GameMaster::GameMaster(
        std::string game_name,
        const secure_string &game_password,
        const secure_string &master_password)
      : _game_name(std::move(game_name)),
        _auth(game_password),
        _master_id(_auth.AddMasterPlayer(game_password, master_password)),
        _world() {}

} // namespace engine
} // namespace mess
