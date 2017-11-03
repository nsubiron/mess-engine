#include "mess/engine/Time.h"

#include <ctime>
#include <mutex>
#include <stdexcept>
#include <string>

namespace mess {
namespace engine {
namespace time {

  static std::tm localtime(const std::time_t time) {
    static std::mutex MUTEX;
    std::lock_guard<std::mutex> lock(MUTEX);
    const std::tm *tm = std::localtime(&time);
    if (tm == NULL)
      throw std::runtime_error("error retrieving local time");
    return *tm;
  }

  std::string strftime(const clock::time_point time, const std::string &format) {
    const std::tm locat_time = localtime(clock::to_time_t(time));
    const size_t buffer_size = 128u;
    char buffer[buffer_size];
    const std::size_t result = std::strftime(buffer, buffer_size, format.c_str(), &locat_time);
    if (result == 0u)
      throw std::runtime_error("error printing time: buffer size too small");
    return buffer;
  }

  std::string to_display_string(const clock::time_point time) {
    return strftime(time, "%F %T");
  }

} // namespace time
} // namespace engine
} // namespace mess
