
#ifndef ARC_ENGINE_ENTITY_H
#define ARC_ENGINE_ENTITY_H
#include "core.h"
#include "scene.h"
namespace arc {
class Scene;
class Entity {
public:
  Entity(entt::entity entity_handle, Scene *scene)
      : entity_handle_(entity_handle), scene_(scene) {}
  Entity(const Entity &other) = default;
  Entity() = default;
  inline void Setup(entt::entity entity_handle, Scene *scene){
    entity_handle_=entity_handle;
    scene_ = scene;
  }
  ~Entity() = default;
  template <typename T> inline bool Has() {
    return scene_->registry().all_of<T>(entity_handle_);
  }

  template <typename T, typename... Args> inline T &Add(Args &&...args) {
    arc_assert(!Has<T>(),
               "Failed Adding Component! Entity already has that component.");
    return scene_->registry().emplace<T>(
        entity_handle_,
        std::forward<Args>(args)...); // TODO: look at how this works
  }

  template <typename T> inline T &Get() {
    arc_assert(Has<T>(),
               "Failed Getting Component! Entity doesn't have that component.");
    return scene_->registry().get<T>(entity_handle_);
  }

  template <typename T> inline void Remove() {
    arc_assert(Has<T>(),
               "Failed Getting Component! Entity doesn't have that component.");
    scene_->registry().remove<T>(entity_handle_);
  }

private:
  entt::entity entity_handle_{entt::null};
  Scene *scene_;
  friend class Scene;
};
} // namespace arc
#endif
