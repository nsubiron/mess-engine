#include "mess/engine/Output.h"

#include "mess/engine/LogItem.h"
#include "mess/engine/PlayerId.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <memory>
#include <stdarg.h>

namespace mess {
namespace engine {

namespace string {
namespace detail {

  // From http://stackoverflow.com/a/8098080
  std::string format(const std::string fmt_str, ...) {
    int n = fmt_str.size() * 2u; /* reserve 2 times as much as the length of the fmt_str */
    std::unique_ptr<char[]> formatted;
    va_list ap;
    for (;;) {
      formatted.reset(new char[n]);   /* wrap the plain char array into the unique_ptr */
      std::strcpy(&formatted[0], fmt_str.c_str());
      va_start(ap, fmt_str);
      const int final_n = vsnprintf(&formatted[0], n, fmt_str.c_str(), ap);
      va_end(ap);
      if (final_n < 0 || final_n >= n) {
        n += std::abs(final_n - n + 1);
      } else {
        break;
      }
    }
    return std::string(formatted.get());
  }

} // namespace detail
} // namespace string

  static const char *to_string(const Access access) {
    switch (access) {
      default:
      case Access::Everyone: return "Everyone";
      case Access::Faction:  return "Faction";
      case Access::Player:   return "Player";
      case Access::Master:   return "Master";
    }
  }

  static const char *to_string(const LogPriority log_priority) {
    switch (log_priority) {
      default:
      case CRITICAL: return "CRITICAL";
      case ERROR:    return "ERROR";
      case WARNING:  return "WARNING";
      case NOTICE:   return "NOTICE";
      case INFO:     return "INFO";
      case DEBUG:    return "DEBUG";
    }
  }

  std::ostream &operator<<(std::ostream &os, const Location &location) {
    os << "Location(x=" << location.x << ",y=" << location.y << ")";
    return os;
  }

  std::ostream &operator<<(std::ostream &os, const Access access) {
    os << to_string(access);
    return os;
  }

  std::ostream &operator<<(std::ostream &os, const LogPriority log_priority) {
    os << to_string(log_priority);
    return os;
  }

  std::ostream &operator<<(std::ostream &os, const PlayerId &id) {
    os << id.to_display_string();
    return os;
  }

  std::ostream &operator<<(std::ostream &os, const LogItem &item) {
    os << time::to_display_string(item.time_stamp)
       << " [" << item.player << "]: "
       << item.priority << ": "
       << "@" << item.access << ' '
       << item.message;
    return os;
  }

} // namespace engine
} // namespace mess
