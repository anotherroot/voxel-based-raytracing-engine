#include "func.h"




#include <glm/gtc/matrix_transform.hpp>
#include "voxel_model.h"
namespace arc{
  void Rotate(Entity entity, float deg, const glm::vec3& axis)
  {
    auto &t = entity.Get<TransformComponent>();
    t.transform = glm::rotate(t.transform, glm::radians(deg), axis); 
  }

  void Translate(Entity entity, const glm::vec3& pos)
  {
    auto &t = entity.Get<TransformComponent>();
    t.transform = glm::translate(t.transform, pos);
   
  }
  void SetPosition(Entity entity, const glm::vec3& pos)
  {
    auto &t = entity.Get<TransformComponent>();
    t.transform[3] = glm::vec4(pos, 1);
  
  }

}
