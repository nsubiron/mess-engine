#pragma once

#include "mess/NonCopyable.h"
#include "mess/WeakPointerList.h"
#include "mess/engine/EngineTick.h"
#include "mess/engine/GlobalLog.h"
#include "mess/engine/ObjectInitializer.h"
#include "mess/engine/ObjectVisitor.h"

#include <boost/make_shared.hpp>

namespace mess {
namespace engine {

  class World : private ObjectVisitor, private NonCopyable {
  public:

    template <typename T, typename ... Args>
    boost::shared_ptr<T> SpawnObject(PlayerId owner, Args&& ... args) {
      ObjectInitializer oi(owner, *this);
      auto obj = boost::make_shared<T>(oi, std::forward<Args>(args)...);
      obj->AcceptVisitor(*this);
      return obj;
    }

    GlobalLog &GetGlobalLog() {
      return _log;
    }

    EngineTick &GetEngineTick() {
      return _tick;
    }

  private:

    virtual void visit(Actor &actor) override;
    virtual void visit(Tickable &tickable) override;

    GlobalLog _log;

    weak_ptr_list<Actor> _actors;

    EngineTick _tick;
  };

} // namespace engine
} // namespace mess
