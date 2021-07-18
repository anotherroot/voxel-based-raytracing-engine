#ifndef ARC_ENGINE_VOXEL_RENDERER_H
#define ARC_ENGINE_VOXEL_RENDERER_H

#include "font.h"
#include "perspective_camera.h"
#include "shader.h"
#include "texture.h"
#include "vertex_array.h"
#include <glm.hpp>

namespace arc {
struct VoxelVertex {
  glm::vec3 pos;
  glm::vec3 tex_coord;
};
struct Material {
  glm::vec3 color;
};
struct VoxelTransform{
  glm::mat4 transform;
  glm::vec3 size;
};
class VoxelRenderer {

public:
  void Init();
  void Destroy();
  void Begin(Shader& shader, const arc::PerspectiveCamera &camera);

  void End(Shader& shader);

  void DrawModel(Shader& shader, const arc::Texture &texture, const std::vector<Material>, const glm::vec3 &pos, const glm::vec3 &size, const glm::vec3 &rotation);
  void SetMaterials(Shader& shader, const std::vector<Material> &materials);
  void DrawTextures(Shader& shader, const std::vector<Texture> & textures, const std::vector<VoxelTransform> &transforms);
  void DrawTexture(Shader& shader, const Texture &textures, const glm::vec3 &pos, const glm::vec3 &rot, const glm::vec3 &size);
  void DrawTest();

private:
  void Flush(Shader& shader);

  uint indices_[36];
  VoxelVertex vertices_[8];
  VertexArray vertex_array_;
  IndexBuffer index_buffer_;
  VertexBuffer vertex_buffer_;
};

} // namespace arc

#endif // ARC_ENGINE_VOXEL_RENDERER_H
