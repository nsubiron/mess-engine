#include "mess/game/Python.h"

#include "mess/engine/Output.h"
#include "mess/engine/Player.h"

#include <boost/python.hpp>
#include <boost/enable_shared_from_this.hpp>

#include <iostream>

namespace mess {
namespace game {

  class PythonGame : public boost::enable_shared_from_this<PythonGame> {
  public:

    void AddPlayerResource(mess::engine::Access /*access*/, std::string name, uint64_t initial_value) {
      std::cout << name << " = " << initial_value << "\n";
    }

    virtual void RegisterPlayer(mess::engine::Player &/*new_player*/) {}
  };

  class PythonGameWrap : public PythonGame, public boost::python::wrapper<PythonGame> {
  public:
    template <typename ... ARGS>
    PythonGameWrap(ARGS&& ... args) : PythonGame(std::forward<ARGS>(args)...) {}

    virtual void RegisterPlayer(mess::engine::Player &new_player) override {
      get_override("register_player")(new_player);
    }
  };

  void python_export_game() {
    using namespace boost::python;
    using namespace mess::engine;

    class_<PythonGameWrap, boost::noncopyable, boost::shared_ptr<PythonGameWrap>>("Game")
        .def("add_player_resource", &PythonGame::AddPlayerResource)
        .def("register_player", &PythonGame::RegisterPlayer)
    ;
  }

} // namespace game
} // namespace mess
