#pragma once

#include "mess/NonCopyable.h"
#include "mess/engine/PlayerId.h"

#include <mutex>
#include <string>

namespace mess {
namespace engine {

  class PlayerCredentials : private NonCopyable {
  public:

    PlayerCredentials(PlayerId id, std::string username)
        : _id(id),
          _username(std::move(username)) {}

    PlayerId player_id() const {
      return _id;
    }

    std::string username() const {
      std::lock_guard<std::mutex> lock(_mutex);
      return _username;
    }

    void ChangeUsername(std::string username) {
      std::lock_guard<std::mutex> lock(_mutex);
      _username = std::move(username);
    }

  private:

    mutable std::mutex _mutex;

    const PlayerId _id;

    std::string _username;
  };

} // namespace engine
} // namespace mess
