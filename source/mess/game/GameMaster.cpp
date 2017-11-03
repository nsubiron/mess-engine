#include "mess/game/GameMaster.h"

#include "mess/game/GameSettings.h"

#include <boost/algorithm/string.hpp>

namespace mess {
namespace game {

  GameMaster::GameMaster(
      const GameSettings &settings,
      const crypto::secure_string &game_password,
      const crypto::secure_string &master_password)
    : mess::engine::GameMaster(settings.game_name, game_password, master_password),
      _python_module(settings.python_code_folder) {}

  std::string GameMaster::HandleRequest(std::string request) {
    try {
      std::vector<std::string> strs;
      boost::split(strs, request, boost::is_any_of("\t "));
      request = strs[0];

      if ((request == "q") || (request == "exit") || (request == "quit")) {
        throw request_exit();
      } else if (request == "new") {
        std::string pwd1 = (strs.size() > 1u ? strs[1u] : "");
        std::string pwd2 = (strs.size() > 2u ? strs[2u] : "");
        auto game_pwd = crypto::secure_string::unsafe_make(pwd1);
        auto player_pwd = crypto::secure_string::unsafe_make(pwd2);
        auto player = AddPlayer(game_pwd, player_pwd, "Unnamed Player");
        _python_module.AddPlayer(*player);
        return "new player created";
      }
      return request;
    } catch (const request_exit &) {
      throw;
    } catch (const std::exception &e) {
      return std::string("error: ") + e.what();
    }
  }

} // namespace game
} // namespace engine
