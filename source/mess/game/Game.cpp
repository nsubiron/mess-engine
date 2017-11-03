#include "mess/game/Game.h"

#include "mess/game/Python.h"

#include <boost/python.hpp>

namespace mess {
namespace game {

  Game::Game(
      boost::asio::io_service &io_service,
      const GameSettings &settings,
      const crypto::secure_string &game_password,
      const crypto::secure_string &master_password)
    : _master(settings, game_password, master_password),
      _engine(
          io_service,
          _master.GetEngineTick(),
          boost::posix_time::milliseconds(1000u * settings.desired_delta_time_in_seconds),
          settings.time_scale),
      _server(io_service, settings.server_port) {
  }

  Game::~Game() {
    Exit();
  }

  void Game::AsyncRun() {
    _server.Listen([&](std::string str){
      try {
        return _master.HandleRequest(std::move(str));
      } catch (const request_exit &exit) {
        Exit();
        return std::string(exit.what());
      }
    });
    _engine.AsyncLoop();
  }

  void Game::Exit() {
    _engine.set_done();
    _server.Close();
  }

} // namespace game
} // namespace mess
