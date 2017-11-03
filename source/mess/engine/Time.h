#pragma once

#include <chrono>
#include <cstdint>
#include <string>
#include <type_traits>

namespace mess {
namespace engine {
namespace time {

  using clock = std::chrono::system_clock;

  inline clock::time_point now() {
    return clock::now();
  }

  inline int64_t seconds_since_epoch(clock::time_point time) {
    static_assert(std::is_same<std::chrono::seconds::rep, int64_t>::value, "invalid time implementation");
    return std::chrono::duration_cast<std::chrono::seconds>(time.time_since_epoch()).count();
  }

  std::string strftime(clock::time_point time, const std::string &format);

  std::string to_display_string(clock::time_point time);

} // namespace time
} // namespace engine
} // namespace mess
