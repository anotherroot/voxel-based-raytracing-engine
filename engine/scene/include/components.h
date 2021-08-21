
#ifndef ARC_ENGINE_COMPONENTS_H
#define ARC_ENGINE_COMPONENTS_H
#include "glm.hpp"
#include "perspective_camera.h"
#include "string"
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
class VoxelModel;
struct ModelComponent {
  VoxelModel *model;
  bool draw{true}, erase{false};
  glm::vec3 voxel_size{0.1,0.1,0.1};
  ModelComponent() = default;
  ModelComponent(const ModelComponent &m) = default;
  ModelComponent(VoxelModel *m):model(m){
  }

};

struct LightComponent{
  float long_range, short_range, radius;
  glm::vec3 color;
  LightComponent() = default;
  LightComponent(const LightComponent &m) = default;
  LightComponent(float lr, float sr, const glm::vec3& col, float rad):long_range(lr), short_range(sr), color(col), radius(rad){
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
