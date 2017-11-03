#include "mess/game/Python.h"

#include "mess/engine/Access.h"
#include "mess/engine/Logger.h"
#include "mess/engine/Math.h"
#include "mess/engine/Object.h"
#include "mess/engine/ObjectInitializer.h"
#include "mess/engine/Output.h"
#include "mess/engine/PlayerId.h"

#include <boost/python.hpp>
#include <boost/python/raw_function.hpp>

#include <iostream>

namespace mess {
namespace game {

  using namespace boost::python;

  static constexpr const char *get_access_keyword() {
    return "__access__";
  }

  void python_export_decorators() {
    using namespace mess::engine;

    std::string submodule_name(extract<const char*>(scope().attr("__name__")));
    submodule_name.append(".decorators");

    // Create the submodule, and attach it to the current scope.
    object submodule(handle<>(borrowed(PyImport_AddModule(submodule_name.c_str()))));
    scope().attr("decorators") = submodule;

    {
      // Switch the scope to the submodule, add methods and classes.
      scope submodule_scope = submodule;

      def("everyone", +[](object obj){ obj.attr(get_access_keyword()) = Access::Everyone; return obj; });
      def("faction",  +[](object obj){ obj.attr(get_access_keyword()) = Access::Faction;  return obj; });
      def("player",   +[](object obj){ obj.attr(get_access_keyword()) = Access::Player;   return obj; });
      def("master",   +[](object obj){ obj.attr(get_access_keyword()) = Access::Master;   return obj; });
    }
  }

  class call_logger {
  private:
    static std::string format(tuple args, size_t begin) {
      return extract<std::string>(args[begin] % args.slice(begin + 1u, len(args)));
    }
  public:
#define LOGGER_CALL(level) \
        static object level(tuple args, dict /*kwargs*/) { \
          mess::engine::Logger &self = extract<mess::engine::Logger&>(args[0u]); \
          const auto &access = extract<mess::engine::Access>(args[1u]); \
          if (access.check()) { \
            self.level(access, format(args, 2u)); \
          } else { \
            self.level(format(args, 1u)); \
          } \
          return object(); \
        }
    LOGGER_CALL(critical)
    LOGGER_CALL(error)
    LOGGER_CALL(warning)
    LOGGER_CALL(notice)
    LOGGER_CALL(info)
#ifdef _DEBUG
    LOGGER_CALL(debug)
#else
    static object debug(tuple /*args*/, dict /*kwargs*/) { return object(); }
#endif // _DEBUG
#undef LOGGER_CALL
  };

  void python_export_engine() {
    using namespace mess::engine;

    enum_<Access>("access")
        .value("Everyone", Access::Everyone)
        .value("Faction",  Access::Faction)
        .value("Player",   Access::Player)
        .value("Master",   Access::Master)
    ;

    python_export_decorators();

    class_<Location>("Location")
        .def(init<Location::value_type, Location::value_type>())
        .add_property(
            "x",
            +[](const Location &self){ return self.x; },
            +[](Location &self, Location::value_type x){ self.x = x; })
        .add_property(
            "y",
            +[](const Location &self){ return self.y; },
            +[](Location &self, Location::value_type y){ self.y = y; })
        .def("is_valid", &Location::IsValid)
        .def(self_ns::str(self_ns::self))
    ;

    class_<PlayerId>("PlayerId", init<const PlayerId &>())
        .def(self_ns::str(self_ns::self))
        .def(self_ns::str == self_ns::self)
        .def(self_ns::str != self_ns::self)
    ;

    class_<Logger>("Logger", no_init)
        .def("__call__", raw_function(&call_logger::info, 2))
        .def("critical", raw_function(&call_logger::critical, 2))
        .def("error", raw_function(&call_logger::error, 2))
        .def("warning", raw_function(&call_logger::warning, 2))
        .def("notice", raw_function(&call_logger::notice, 2))
        .def("info", raw_function(&call_logger::info, 2))
        .def("debug", raw_function(&call_logger::debug, 2))
    ;

    class_<ObjectInitializer, boost::noncopyable>("ObjectInitializer", no_init);

    class_<Object, boost::noncopyable, boost::shared_ptr<Object>>("GameObject", no_init)
        .def("get_owner_id", &Object::GetOwnerId)
        .def("change_owner", &Object::ChangeOwner)
        .def("change_owner", +[](Object &self, const Object &parent){ self.ChangeOwner(parent.GetOwnerId()); })
        .add_property("visibility", &Object::GetVisibility, &Object::SetVisibility)
        .def_readonly("log", &Object::logger)
    ;
  }

} // namespace game
} // namespace mess
