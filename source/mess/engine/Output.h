#pragma once

#include "crypto/output.h"
#include "mess/engine/Access.h"
#include "mess/engine/LogPriority.h"
#include "mess/engine/Math.h"

#include <ostream>
#include <string>

namespace mess {
namespace engine {

  class LogItem;
  class PlayerId;

namespace string {
namespace detail {

  template <typename T>
  inline auto parse(const T &arg) {
    return arg;
  }

  template <>
  inline auto parse(const std::string &arg) {
    return arg.c_str();
  }

  std::string format(std::string format, ...);

} // namespace detail

  template<typename ... Args>
  std::string format(const std::string &format, Args ... args) {
    return detail::format(format, detail::parse(args)...);
  }

  inline std::string to_hex_string(const unsigned char *data, size_t length) {
    return ::crypto::to_hex_string(::crypto::buffer_view::make_const(data, length));
  }

} // namespace string

  std::ostream &operator<<(std::ostream &os, const Location &location);

  std::ostream &operator<<(std::ostream &os, Access access);

  std::ostream &operator<<(std::ostream &os, LogPriority log_priority);

  std::ostream &operator<<(std::ostream &os, const PlayerId &id);

  std::ostream &operator<<(std::ostream &os, const LogItem &log_item);

} // namespace engine
} // namespace mess
