#include "mess/game/Python.h"

#include "mess/engine/Player.h"

#include <boost/python.hpp>

namespace mess {
namespace game {

  void python_export_player() {
    using namespace boost::python;
    using namespace mess::engine;

    class_<Player, boost::noncopyable, bases<Object>>("Player", no_init)
        .def_readonly("id", &Player::player_id)
        .def_readonly("username", &Player::username)
    ;
  }

} // namespace game
} // namespace mess
