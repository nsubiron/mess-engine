#pragma once

#include "mess/Debug.h"
#include "mess/NonCopyable.h"

#include <cstdio>
#include <memory>
#include <string>

namespace mess {
namespace engine {
namespace logging {

namespace detail {

  template<typename ... Args>
  std::string format_string(const std::string &format, Args ... args) {
    // https://stackoverflow.com/a/26221725.
    size_t size = std::snprintf(nullptr, 0, format.c_str(), args ...) + 1;
    std::unique_ptr<char[]> buffer(new char[size]);
    std::snprintf(buffer.get(), size, format.c_str(), args ...);
    return std::string(buffer.get(), buffer.get() + size - 1);
  }

} // namespace detail

#ifdef _DEBUG

  template <typename ... Args>
  static inline void debug(const std::string &format, Args &&...) {
    std::cout << detail::format_string("DEBUG: " + format, std::forward<Args>(args)...);
  }

#else

  template <typename ... Args>
  static inline void debug(Args &&...) {}

#endif // _DEBUG

} // namespace logging
} // namespace engine
} // namespace mess
