#pragma once

#include "mess/engine/Output.h"

#include <cstdint>

namespace mess {
namespace engine {

  class PlayerId {
  public:

    PlayerId(const PlayerId &rhs) : _id(rhs._id) {}

    void operator=(const PlayerId &rhs) {
      _id = rhs._id;
    }

    bool operator==(const PlayerId &rhs) const {
      return _id == rhs._id;
    }

    bool operator!=(const PlayerId &rhs) const {
      return _id != rhs._id;
    }

    std::string to_display_string() const {
      return string::to_hex_string(reinterpret_cast<const unsigned char*>(&_id), 3u);
    }

  private:

    friend class Security;

    using value_type = uint64_t;

    PlayerId(value_type id) : _id(id) {}

    value_type _id;
  };

} // namespace engine
} // namespace mess
