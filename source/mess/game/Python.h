#pragma once

#include "mess/NonCopyable.h"
#include "mess/engine/Logger.h"

#include <boost/python/object.hpp>

#include <string>

namespace mess {
namespace game {

  class Python : private NonCopyable {
  public:

    using object = boost::python::object;

    static void InitPython();

    static int PrintError();

    static std::string GetName(const object &obj);

    template <typename ... ARGS>
    static object Call(mess::engine::Logger logger, object &function, ARGS&&...args) {
      return CallWrap(logger, function, [&](object &f){ return f(std::forward<ARGS>(args)...); });
    }

  private:

    static object CallWrap(mess::engine::Logger logger, object &obj, std::function<object(object&)> function);
  };

} // namespace game
} // namespace mess
