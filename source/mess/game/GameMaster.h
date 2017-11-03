#pragma once

#include "mess/engine/GameMaster.h"
#include "mess/game/PythonModule.h"

#include <stdexcept>

namespace mess {
namespace game {

  struct GameSettings;

  class request_exit : public std::runtime_error {
  public:
    request_exit() : std::runtime_error("exit requested") {}
  };

  class GameMaster : public mess::engine::GameMaster {
  public:

    explicit GameMaster(
        const GameSettings &settings,
        const crypto::secure_string &game_password,
        const crypto::secure_string &master_password);

    std::string HandleRequest(std::string request);

  private:

    PythonModule _python_module;
  };

} // namespace game
} // namespace engine
