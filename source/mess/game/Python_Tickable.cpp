#include "mess/game/Python.h"

#include "mess/engine/Actor.h"
#include "mess/engine/ActorComponent.h"
#include "mess/engine/MovementComponent.h"

#include <boost/python.hpp>

namespace mess {
namespace game {

  template <typename T>
  struct DataHolder {
    T data;
  };

  template <>
  struct DataHolder<std::nullptr_t> {};

  // ===========================================================================
  // -- TickableWrap -----------------------------------------------------------
  // ===========================================================================

  template <typename T, typename DATA = std::nullptr_t>
  class TickableWrap
    : public T,
      public boost::python::wrapper<T>,
      public DataHolder<DATA> {
    MESS_OBJECT_BODY(TickableWrap, T)
    using Wrap = boost::python::wrapper<T>;
    using Self = TickableWrap<T, DATA>;
  private:

    template <bool only_if_present, typename ... ARGS>
    inline void Call(const char *name, ARGS&&...args) {
      auto func = Wrap::get_override(name);
      if constexpr (only_if_present) {
        if (!func)
          return;
      }
      Python::Call(mess::engine::Object::logger(), func, std::forward<ARGS>(args)...);
    }

    constexpr static bool OnlyIfPresent = true;

  public:

    template <typename ... ARGS>
    TickableWrap(const mess::engine::ObjectInitializer &oi, bool can_tick, ARGS&& ... args)
      : Super(oi, std::forward<ARGS>(args)...) {
      mess::engine::Tickable::can_ever_tick = can_tick;
    }

    virtual void BeginPlay() final {
      Super::BeginPlay();
      Call<OnlyIfPresent>("begin_play");
    }

    virtual void Tick(float delta_time) final {
      Super::Tick(delta_time);
      Call<!OnlyIfPresent>("tick", delta_time);
    }

    virtual void EndPlay() final {
      Super::EndPlay();
      Call<OnlyIfPresent>("end_play");
    }

    template <typename F, typename ... ARGS>
    static auto Make(boost::python::object self, F base_constructor, ARGS&&...args) {
      using namespace boost::python;
      auto constructor = make_constructor(base_constructor);
      const bool can_tick = PyObject_HasAttrString(self.ptr(), "tick");
      constructor(self, can_tick, std::forward<ARGS>(args)...);
      // For some reason we need to initialize the wrapper here.
      boost::shared_ptr<Self> ptr = extract<boost::shared_ptr<Self>>(self);
      initialize_wrapper(self.ptr(), get_pointer(ptr));
      return self;
    }
  };

  // ===========================================================================
  // -- ActorWrap --------------------------------------------------------------
  // ===========================================================================

  using ActorWrap = TickableWrap<mess::engine::Actor>;

  static auto MakeActor(
      boost::python::object self,
      boost::shared_ptr<mess::engine::Object> parent,
      std::string name,
      std::string type) {
    auto base_constructor = +[](
        bool can_tick,
        const mess::engine::Object &parent,
        std::string name,
        std::string type) {
      return parent.SpawnObject<ActorWrap>(can_tick, std::move(name), std::move(type));
    };
    return ActorWrap::Make(self, base_constructor, boost::ref(*parent), std::move(name), std::move(type));
  }

  // ===========================================================================
  // -- ActorComponentWrap -----------------------------------------------------
  // ===========================================================================

  using ActorComponentWrap = TickableWrap<mess::engine::ActorComponent, boost::weak_ptr<ActorWrap>>;

  // This just creates a constructor to reorder its arguments.
  struct ActorComponentWrapConstructor : public ActorComponentWrap {
    ActorComponentWrapConstructor(
      const mess::engine::ObjectInitializer &oi,
      mess::engine::Actor &parent,
      bool can_tick)
      : ActorComponentWrap(oi, can_tick, parent) {}
  };

  static auto MakeActorComponent(
      boost::python::object self,
      boost::shared_ptr<ActorWrap> parent) {
    auto base_constructor = +[](
        bool can_tick,
        ActorWrap &parent) {
      boost::shared_ptr<ActorComponentWrap> ptr =
          parent.SpawnComponent<ActorComponentWrapConstructor>(can_tick);
      ptr->data = parent.WeakFromThis();
      return ptr;
    };
    return ActorComponentWrapConstructor::Make(self, base_constructor, boost::ref(*parent));
  }

  // ===========================================================================
  // -- python_export_tickable -------------------------------------------------
  // ===========================================================================

  void python_export_tickable() {
    using namespace boost::python;
    using namespace mess::engine;

    class_<ActorWrap, boost::noncopyable, bases<Object>, boost::shared_ptr<ActorWrap>>("Actor", no_init)
        .def("__init__", make_function(&MakeActor))
        .add_property("name", +[](const ActorWrap &self){ return self.GetName(); }, &Actor::SetName)
        .add_property("type", +[](const ActorWrap &self){ return self.GetType(); }, &Actor::SetType)
        .add_property("enable_tick", &Tickable::IsTickEnabled, &Tickable::EnableTick)
        .def("begin_play", &Tickable::BeginPlay)
        .def("end_play", &Tickable::EndPlay)
        .add_property("location", &Positionable::GetLocation, &Positionable::SetLocation)
        .def("is_at_location", &Positionable::IsAtLocation)
        .def("is_at_location", +[](const ActorWrap &self, Location::value_type x, Location::value_type y){ return self.IsAtLocation(Location(x, y)); })
    ;

    class_<ActorComponentWrap, boost::noncopyable, bases<Object>, boost::shared_ptr<ActorComponentWrap>>("ActorComponent", no_init)
        .def("__init__", make_function(&MakeActorComponent))
        .add_property("enable_tick", &Tickable::IsTickEnabled, &Tickable::EnableTick)
        .def("begin_play", &Tickable::BeginPlay)
        .def("end_play", &Tickable::EndPlay)
        .def("get_actor", +[](const ActorComponentWrap &self){ return self.data.lock(); })
    ;
  }

} // namespace game
} // namespace mess
