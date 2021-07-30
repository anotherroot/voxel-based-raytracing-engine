
#ifndef ARC_ENGINE_SCENE_H
#define ARC_ENGINE_SCENE_H
#include "entt/entt.hpp"
namespace arc {
class Entity;
class Scene {

public:
  void Setup(const std::string &name); // returns the main camera
  void Dispose();
  ~Scene() { Dispose(); }
  operator bool() const { return setup_; }

  Entity CreateEntity(const std::string &name);
  void DestroyEntity(const Entity &entity);
  inline entt::registry &registry() { return registry_; }

private:
  int next_id_{0};
  bool setup_{false};
  entt::registry registry_;
};

} // namespace arc

#endif
