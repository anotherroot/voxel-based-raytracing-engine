
#ifndef ARC_ENGINE_COMPONENTS_H
#define ARC_ENGINE_COMPONENTS_H
#include "glm.hpp"
#include "perspective_camera.h"
#include "string"
#include "structs.h"
namespace arc {
  
struct IDComponent {
  int id;
  IDComponent() = default;
  IDComponent(const IDComponent &t) = default;
  IDComponent(int i) : id(i) {}
};

struct TagComponent {
  std::string tag;
  TagComponent() = default;
  TagComponent(const TagComponent &t) = default;
  TagComponent(const std::string &s) : tag(s) {}
};
struct TransformComponent {
  glm::mat4 transform;
  TransformComponent() = default;
  TransformComponent(const TransformComponent &t) = default;
  TransformComponent(const glm::mat4 &t) : transform(t) {}

};
struct ModelComponent {
  
  glm::vec3 voxel_size{0.1,0.1,0.1}, raw_size, size;
  int id;
  bool changed{false};
  ModelComponent() = default;
  ModelComponent(const ModelComponent &m) = default;
  ModelComponent(int i, const glm::vec3& vs, const glm::vec3& rs, const glm::vec3& s):id(i), voxel_size(vs), raw_size(rs), size(s){
  }

};
struct MaterialComponent{
  Material mat;
  bool changed{false};
  MaterialComponent() = default;
  MaterialComponent(const MaterialComponent &m) = default;
  MaterialComponent(const Material &m):mat(m){
  }
};

struct LightComponent{
  float range, radius;
  glm::vec3 color;
  bool moded{0};
  LightComponent() = default;
  LightComponent(const LightComponent &m) = default;
  LightComponent(float r, const glm::vec3& col, float rad):range(r), color(col), radius(rad){
  }
};


struct BoxComponent{
  glm::vec3 color, size;
  BoxComponent() = default;
  BoxComponent(const BoxComponent &m) = default;
  BoxComponent(const glm::vec3& col,const glm::vec3& s):color(col),size(s){
  }
};

struct CameraComponent {
  glm::mat4 projection;
  bool primary;
  CameraComponent() = default;
  CameraComponent(const CameraComponent &c) = default;
  CameraComponent(const glm::mat4& pro, bool prim) :projection(pro), primary(prim){}
};

} // namespace arc
#endif
