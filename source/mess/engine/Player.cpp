#include "mess/engine/Player.h"

namespace mess {
namespace engine {

  Player::Player(const ObjectInitializer &oi, std::string username)
      : Super(oi),
        _credentials(oi.GetPlayerId(), std::move(username)),
        _resources(10u) {
    logger().log(TR_NEW_PLAYER_JOINED, _credentials.username());
  }

} // namespace engine
} // namespace mess
