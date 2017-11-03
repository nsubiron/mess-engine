#pragma once

#include "mess/engine/GlobalLog.h"
#include "mess/engine/I18n.h"
#include "mess/engine/LogItem.h"
#include "mess/engine/Output.h"

#include <boost/make_shared.hpp>

namespace mess {
namespace engine {
namespace detail {

  template <typename T>
  class LoggerTmpl {
  public:

    LoggerTmpl(const T &object) : _obj(object) {}

    template <typename ... Args>
    void log(Access access, LogPriority priority, const std::string &format, Args &&... args) const {
      auto &log = _obj.GetWorld().GetGlobalLog();
      log.Log(boost::make_shared<LogItem>(
          _obj.GetOwnerId(), access, priority, string::format(format, std::forward<Args>(args)...)));
    }

    template <typename ... Args>
    void log(LogPriority priority, const std::string &format, Args &&... args) const {
      log(_obj.GetVisibility(), priority, format, std::forward<Args>(args)...);
    }

    template <typename ... Args>
    void log(const I18nMessage &i18n, Args &&... args) const {
      log(i18n.access, i18n.priority, i18n.message, std::forward<Args>(args)...);
    }

    template <typename ... Args>
    void critical(Access access, const std::string &format, Args &&... args) const {
      log(access, CRITICAL, format, std::forward<Args>(args)...);
    }

    template <typename ... Args>
    void critical(const std::string &format, Args &&... args) const {
      log(Access::Everyone, CRITICAL, format, std::forward<Args>(args)...);
    }

    template <typename ... Args>
    void error(Access access, const std::string &format, Args &&... args) const {
      log(access, ERROR, format, std::forward<Args>(args)...);
    }

    template <typename ... Args>
    void error(const std::string &format, Args &&... args) const {
      log(ERROR, format, std::forward<Args>(args)...);
    }

    template <typename ... Args>
    void warning(Access access, const std::string &format, Args &&... args) const {
      log(access, WARNING, format, std::forward<Args>(args)...);
    }

    template <typename ... Args>
    void warning(const std::string &format, Args &&... args) const {
      log(WARNING, format, std::forward<Args>(args)...);
    }

    template <typename ... Args>
    void notice(Access access, const std::string &format, Args &&... args) const {
      log(access, NOTICE, format, std::forward<Args>(args)...);
    }

    template <typename ... Args>
    void notice(const std::string &format, Args &&... args) const {
      log(NOTICE, format, std::forward<Args>(args)...);
    }

    template <typename ... Args>
    void info(Access access, const std::string &format, Args &&... args) const {
      log(access, INFO, format, std::forward<Args>(args)...);
    }

    template <typename ... Args>
    void info(const std::string &format, Args &&... args) const {
      log(INFO, format, std::forward<Args>(args)...);
    }

#ifdef _DEBUG

    template <typename ... Args>
    void debug(Access access, const std::string &format, Args &&... args) const {
      log(access, DEBUG, format, std::forward<Args>(args)...);
    }

    template <typename ... Args>
    void debug(const std::string &format, Args &&... args) const {
      log(Access::Master, DEBUG, format, std::forward<Args>(args)...);
    }

#else

    template <typename ... Args>
    void debug(Access, const std::string &, Args &&...) const {}

    template <typename ... Args>
    void debug(const std::string &, Args &&...) const {}

#endif // _DEBUG

  private:

    const T &_obj;
  };

} // namespace detail

  class Object;

  using Logger = detail::LoggerTmpl<Object>;

} // namespace engine
} // namespace mess

// Including object here to avoid circular referencing.
#include "mess/engine/Object.h"
