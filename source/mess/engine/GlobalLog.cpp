#include "mess/engine/GlobalLog.h"

#include "mess/Debug.h"
#include "mess/NonCopyable.h"
#include "mess/engine/Output.h"

#include <iostream>

namespace mess {
namespace engine {

  static void PrintToStdOut(const LogItem &item) {
    if (item.priority < WARNING) {
      std::cerr << "\x1b[0;31m" << item << "\x1b[0m\n";
    } else if (item.priority == WARNING) {
      std::cerr << "\x1b[0;33m" << item << "\x1b[0m\n";
    } else if (item.priority > INFO) {
      std::cerr << "\x1b[0;90m" << item << "\x1b[0m\n";
    } else {
      std::cout << item << '\n';
    }
  }

  void GlobalLog::Log(boost::shared_ptr<const LogItem> item) {
    DEBUG_ASSERT(item != nullptr);
    std::lock_guard<std::mutex> lock(_mutex);
    PrintToStdOut(*item);
    _log.push_back(std::move(item));
  }

  GlobalLog::LogList GlobalLog::GetLogSince(size_t index) const {
    std::lock_guard<std::mutex> lock(_mutex);
    if (index >= _log.size()) {
      return _log;
    } else {
      auto begin = _log.begin();
      std::advance(begin, index);
      return LogList(begin, _log.end());
    }
  }

} // namespace engine
} // namespace mess
