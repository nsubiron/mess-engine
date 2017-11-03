#pragma once

#include "mess/engine/Access.h"
#include "mess/engine/LogItem.h"

namespace mess {
namespace engine {

  struct I18nMessage {
    Access access;
    LogPriority priority;
    const std::string &message;
  };

} // namespace engine

#define TR(key, access, priority, str) \
  namespace engine::detail { extern std::string TR_STR_ ## key; } \
  static inline mess::engine::I18nMessage TR_ ## key{ \
      ::mess::engine::Access:: access, \
      ::mess::engine:: priority, \
      ::mess::engine::detail::TR_STR_ ## key};
#include "Messages.in"
#undef TR

} // namespace mess
