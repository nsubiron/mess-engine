#pragma once

#include "mess/engine/Engine.h"
#include "mess/game/GameMaster.h"
#include "mess/game/GameSettings.h"
#include "mess/networking/Server.h"

#include <boost/asio/io_service.hpp>
#include <boost/core/noncopyable.hpp>

#include <cstdint>
#include <memory>
#include <string>

namespace mess {
namespace game {

  class Game : private boost::noncopyable {
  public:

    explicit Game(
        boost::asio::io_service &io_service,
        const GameSettings &settings,
        const crypto::secure_string &game_password,
        const crypto::secure_string &master_password);

    ~Game();

    void AsyncRun();

    void Exit();

  private:

    mess::game::GameMaster _master;

    mess::engine::Engine _engine;

    mess::networking::Server _server;
  };

} // namespace game
} // namespace mess
