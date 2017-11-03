#pragma once

#include "mess/NonCopyable.h"

#include <memory>
#include <string>

namespace mess::engine { class Player; }

namespace mess {
namespace game {

  class PythonModule : private NonCopyable {
  public:

    /// This also initializes python.
    explicit PythonModule(const std::string &folder);

    ~PythonModule();

    void AddPlayer(mess::engine::Player &new_player);

  private:

    class Pimpl;
    const std::unique_ptr<Pimpl> _pimpl;
  };

} // namespace game
} // namespace mess
