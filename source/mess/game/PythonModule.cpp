#include "mess/game/PythonModule.h"

#include "mess/engine/Player.h"
#include "mess/game/Python.h"

#include <boost/python.hpp>

#include <cstdlib> // setenv

namespace mess {
namespace game {

  using namespace boost::python;

  static object LoadModule(const std::string &folder) {
    setenv("PYTHONPATH", folder.c_str(), 1);
    Python::InitPython();
    return import("game");
  }

  static object LoadMasterObject(object module) {
    return module.attr("GameMaster")();
  }

  class PythonModule::Pimpl {
  public:

    Pimpl(const std::string &folder)
    : module(LoadModule(folder)),
      master_object(LoadMasterObject(module)) {}

    object module;

    object master_object;
  };

  PythonModule::PythonModule(const std::string &folder)
    : _pimpl(std::make_unique<Pimpl>(folder)) {}

  PythonModule::~PythonModule() {}

  void PythonModule::AddPlayer(mess::engine::Player &new_player) {
    _pimpl->master_object.attr("register_player")(boost::ref(new_player));
  }

} // namespace game
} // namespace mess
