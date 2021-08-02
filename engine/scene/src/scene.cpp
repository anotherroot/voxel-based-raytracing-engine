
#include "scene.h"
#include "entity.h"
#include "components.h"
namespace arc {
  void Scene::Setup(const std::string& name){
    auto scene = CreateEntity(name);
    auto camera = CreateEntity("Main Camera"); 
    camera.Add<CameraComponent>();
    /* return camera; */
  }
  void Scene::Dispose(){
  }

  Entity Scene::CreateEntity(const std::string &name){
    Entity entity = {registry_.create(),this};
    entity.Add<TagComponent>(name);
    entity.Add<IDComponent>(next_id_++);
    entity.Add<TransformComponent>(glm::mat4(1));
    return entity; 
  }
  void Scene::DestroyEntity(const Entity& entity){
  }

}
