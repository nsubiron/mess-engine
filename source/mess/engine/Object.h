#pragma once

#include "mess/Debug.h"
#include "mess/NonCopyable.h"
#include "mess/engine/Access.h"
#include "mess/engine/Logger.h"
#include "mess/engine/ObjectVisitor.h"
#include "mess/engine/PlayerId.h"
#include "mess/engine/World.h"

#include <boost/enable_shared_from_this.hpp>

namespace mess {
namespace engine {

  class Object
    : public boost::enable_shared_from_this<Object>,
      private NonCopyable {
  protected:

    Object(const ObjectInitializer &oi);

  public:

    virtual ~Object() {}

    PlayerId GetOwnerId() const {
      return _owner_id;
    }

    void ChangeOwner(const PlayerId &new_owner_id) {
      _owner_id = new_owner_id;
    }

    Access GetVisibility() const {
      return _visibility;
    }

    void SetVisibility(Access visibility) {
      _visibility = visibility;
    }

    World &GetWorld() const {
      return _world;
    }

    virtual void AcceptVisitor(ObjectVisitor &) {}

  // protected:

    template <typename T, typename ... ARGS>
    boost::shared_ptr<T> SpawnObject(ARGS&& ... args) const {
      return GetWorld().SpawnObject<T>(GetOwnerId(), std::forward<ARGS>(args)...);
    }

    Logger logger() const {
      return *this;
    }

  private:

    const uint64_t _unique_id;

    PlayerId _owner_id;

    Access _visibility = Access::Player;

    World &_world;
  };

} // namespace engine
} // namespace mess

#define MESS_OBJECT_BODY(cls, super) \
  private: \
    using Super = super; \
  public: \
    boost::shared_ptr<cls> SharedFromThis() { return boost::static_pointer_cast<cls>(Super::shared_from_this()); } \
    boost::shared_ptr<cls const> SharedFromThis() const { return boost::static_pointer_cast<cls const>(Super::shared_from_this()); } \
    boost::weak_ptr<cls> WeakFromThis() { return SharedFromThis(); } \
    boost::weak_ptr<cls const> WeakFromThis() const { return SharedFromThis(); } \
    virtual void AcceptVisitor(mess::engine::ObjectVisitor &visitor) override { Super::AcceptVisitor(visitor); visitor.visit(*this); } \
  private:
