#pragma once

#include "mess/Debug.h"
#include "mess/NonCopyable.h"

#include <algorithm>
#include <cstdint>
#include <vector>

namespace mess {
namespace engine {
namespace detail {

  class Resource {
  public:

    using value_type = uint64_t;

    value_type value() const {
      return _value;
    }

    void set_value(value_type value) {
      _value = value;
    }

    value_type Request(value_type count) {
      const auto result = std::min(_value, count);
      _value -= result;
      return result;
    }

    void Add(value_type count) {
      _value += count;
    }

    bool operator<(Resource rhs) const {
      return _value < rhs._value;
    }

  private:

    value_type _value = 0u;
  };

} // namespace detail

  class Resources : private NonCopyable {
  public:

    using value_type = detail::Resource::value_type;

    explicit Resources(size_t count) : _resources(count) {}

    explicit Resources(Resources &&rhs) : _resources(std::move(rhs._resources)) {}

    size_t size() const {
      return _resources.size();
    }

    value_type GetValue(size_t key) const {
      return _resources[key].value();
    }

    void SetValue(size_t key, value_type value) {
      _resources[key].set_value(value);
    }

    // value_type Request(size_t key, value_type count) {
    //   return _resources[key].Request(count);
    // }

    // Fail if it doesn't have enough of any of the requested resources. Returns
    // wether it took the resources or not.
    bool Request(const Resources &rhs) {
      DEBUG_ASSERT(size() == rhs.size());
      for (auto i = 0u; i < size(); ++i) {
        if (_resources[i] < rhs._resources[i])
          return false;
      }
      for (auto i = 0u; i < size(); ++i) {
        DEBUG_ONLY(auto value = ) _resources[i].Request(rhs.GetValue(i));
        DEBUG_ASSERT(value == rhs.GetValue(i));
      }
      return true;
    }

    // void Add(size_t key, value_type count) {
    //   _resources[key].Add(count);
    // }

    void Add(const Resources &rhs) {
      DEBUG_ASSERT(size() == rhs.size());
      for (auto i = 0u; i < size(); ++i) {
        _resources[i].Add(rhs.GetValue(i));
      }
    }

  private:

    std::vector<detail::Resource> _resources;
  };

} // namespace engine
} // namespace mess
