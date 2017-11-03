#pragma once

#include <list>
#include <mutex>
#include <type_traits>

#include <boost/shared_ptr.hpp>

namespace mess {

  /// Holds a list of objects without owning the objects.
  template <typename T>
  class weak_ptr_list {
  public:

    void push_back(boost::weak_ptr<T> ptr) {
      std::lock_guard<std::mutex> lock(_mutex);
      _list.push_back(ptr);
    }

    /// For each non-null element in the list call @a func. Removes any nullptr
    /// from the list. It is important that @a func does not modify the list,
    /// otherwise a dead-lock will occur.
    template <typename F>
    void for_each(F func) {
      std::lock_guard<std::mutex> lock(_mutex);
      for (auto it = _list.begin(); it != _list.end();) {
        auto ptr = it->lock();
        if (ptr == nullptr) {
          it = _list.erase(it);
        } else {
          func(*ptr);
          ++it;
        }
      }
    }

  private:

    mutable std::mutex _mutex;

    std::list<boost::weak_ptr<T>> _list;
  };

} // namespace mess
