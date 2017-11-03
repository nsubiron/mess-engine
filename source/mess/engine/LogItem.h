#pragma once

#include "mess/engine/Access.h"
#include "mess/engine/LogPriority.h"
#include "mess/engine/PlayerId.h"
#include "mess/engine/Time.h"

namespace mess {
namespace engine {

  class LogItem {
  public:

    LogItem(
        PlayerId player,
        Access access,
        LogPriority priority,
        std::string message)
      : time_stamp(time::now()),
        player(player),
        access(access),
        priority(priority),
        message(std::move(message)) {}

    const time::clock::time_point time_stamp;

    const PlayerId player;

    const Access access;

    const LogPriority priority;

    const std::string message;
  };

} // namespace engine
} // namespace mess
