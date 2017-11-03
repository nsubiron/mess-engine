#include "mess/game/Python.h"

#include "mess/engine/I18n.h"

#include <boost/python.hpp>

#if PY_VERSION_HEX >= 0x03000000
#  define MODULE_INIT_FN(name) BOOST_PP_CAT(PyInit_, name)
#else
#  define MODULE_INIT_FN(name) BOOST_PP_CAT(init, name)
#endif

namespace mess {
namespace game {

  using namespace boost::python;

  void python_export_engine();
  void python_export_player();
  void python_export_tickable();
  void python_export_game();

  // ===========================================================================
  // -- mess module ------------------------------------------------------------
  // ===========================================================================

  BOOST_PYTHON_MODULE(mess) {
    scope().attr("__path__") = "mess";
    python_export_engine();
    python_export_player();
    python_export_tickable();
    python_export_game();
  }

  // ===========================================================================
  // -- Python -----------------------------------------------------------------
  // ===========================================================================

  void Python::InitPython() {
    PyImport_AppendInittab("mess", &MODULE_INIT_FN(mess));
    Py_Initialize();
  }

  int Python::PrintError() {
    PyErr_Print();
    return 42;
  }

  std::string Python::GetName(const object &obj) {
    auto name_attr = obj.attr("__name__");
    if (name_attr)
      return extract<std::string>(name_attr);
    return "";
  }

  object Python::CallWrap(
      mess::engine::Logger logger,
      object &obj,
      std::function<object(object&)> function) {
    try {
      return function(obj);
    } catch (const boost::python::error_already_set &) {
      logger.log(TR_PYTHON_EXCEPTION_THROWN, GetName(obj));
      PrintError();
    } catch (const std::exception &e) {
      logger.log(TR_FATAL_PYTHON_EXCEPTION_THROWN, GetName(obj), e.what());
      throw;
    } catch (...) {
      logger.log(TR_FATAL_PYTHON_EXCEPTION_THROWN, GetName(obj), "unknown exception");
      throw;
    }
    return object();
  }

} // namespace game
} // namespace mess
