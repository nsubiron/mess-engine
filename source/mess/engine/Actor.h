#pragma once

#include "mess/WeakPointerList.h"
#include "mess/engine/ActorComponent.h"
#include "mess/engine/Positionable.h"
#include "mess/engine/Tickable.h"

namespace mess {
namespace engine {

  class Actor : public Tickable, public Positionable {
    MESS_OBJECT_BODY(Actor, Tickable)
  public:

    explicit Actor(const ObjectInitializer &oi);

    explicit Actor(const ObjectInitializer &oi, std::string name);

    explicit Actor(const ObjectInitializer &oi, std::string name, std::string type);

    ~Actor();

    template <typename T, typename ... Args>
    boost::shared_ptr<T> SpawnComponent(Args&& ... args) {
      auto component = SpawnObject<T>(*this, std::forward<Args>(args)...);
      _components.push_back(component);
      return component;
    }

    const std::string &GetName() const {
      return _name;
    }

    void SetName(std::string name) {
      _name = std::move(name);
    }

    const std::string &GetType() const {
      return _type;
    }

    void SetType(std::string type) {
      _type = std::move(type);
    }

  private:

    std::string _name;

    std::string _type;

    weak_ptr_list<ActorComponent> _components;
  };

} // namespace engine
} // namespace mess
