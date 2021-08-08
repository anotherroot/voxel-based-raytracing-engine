#ifndef FUNC_H
#define FUNC_H
#include "entity.h"
#include "components.h"
namespace arc{
  void Rotate(Entity entity, float deg, const glm::vec3& axis);
  void Translate(Entity entity, const glm::vec3& pos);
  void SetPosition(Entity entity, const glm::vec3& pos);
}
#endif
