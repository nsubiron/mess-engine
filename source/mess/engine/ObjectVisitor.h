#pragma once

namespace mess {
namespace engine {

  class Actor;
  class ActorComponent;
  class Object;
  class Player;
  class Tickable;

  class ObjectVisitor {
  public:

    virtual void visit(Actor &) {}
    virtual void visit(ActorComponent &) {}
    virtual void visit(Object &) {}
    virtual void visit(Player &) {}
    virtual void visit(Tickable &) {}
  };

} // namespace engine
} // namespace mess
