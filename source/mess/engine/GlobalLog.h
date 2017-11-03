#pragma once

#include "mess/NonCopyable.h"
#include "mess/engine/LogItem.h"

#include <boost/shared_ptr.hpp>

#include <list>
#include <mutex>

namespace mess {
namespace engine {

  class GlobalLog : private NonCopyable {
  public:

    using LogList = std::list<boost::shared_ptr<const LogItem>>;

    void Log(boost::shared_ptr<const LogItem> item);

    LogList GetLogSince(size_t index) const;

  private:

    mutable std::mutex _mutex;

    LogList _log;
  };

} // namespace engine
} // namespace mess
