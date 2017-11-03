#pragma once

#include <string>

namespace mess {
namespace game {

  struct GameSettings {
    std::string game_name;
    std::string python_code_folder = "./game";
    float desired_delta_time_in_seconds = 1.0f;
    float time_scale = 1.0f;
    uint32_t server_port = 2017u;
  };

} // namespace game
} // namespace mess
