#pragma once

#include "mess/NonCopyable.h"

#include <algorithm>
#include <cstdint>
#include <unordered_map>

namespace mess {
namespace engine {
namespace detail {

  class Stat {
  public:

    using value_type = uint64_t;

    value_type value() const {
      return _value;
    }

    void set_value(value_type value) {
      _value = std::min(value, _maximum);
    }

    value_type maximum() const {
      return _maximum;
    }

    void set_maximum(value_type value) {
      _maximum = value;
    }

    void Subtract(value_type count) {
      _value -= std::min(_value, count);
    }

    void Add(value_type count) {
      set_value(_value + count);
    }

  private:

    value_type _value = 0u;

    value_type _maximum = 100u;
  };

} // namespace detail

  class Stats : private NonCopyable {
  public:

    using value_type = detail::Stat::value_type;

    value_type GetValue(const std::string &key) const {
      return _map[key].value();
    }

    void SetValue(const std::string &key, value_type value) {
      _map[key].set_value(value);
    }

    value_type GetMaximum(const std::string &key) const {
      return _map[key].maximum();
    }

    void SetMaximum(const std::string &key, value_type max) {
      _map[key].set_maximum(max);
    }

    void Subtract(const std::string &key, value_type count) {
      _map[key].Subtract(count);
    }

    void Add(const std::string &key, value_type count) {
      _map[key].Add(count);
    }

  private:

    mutable std::unordered_map<std::string, detail::Stat> _map;
  };

} // namespace engine
} // namespace mess
