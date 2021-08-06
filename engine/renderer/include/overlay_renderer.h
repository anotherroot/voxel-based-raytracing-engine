#ifndef ARC_ENGINE_OVERLAY_RENDERER_H
#define ARC_ENGINE_OVERLAY_RENDERER_H

#include "shader.h"
#include "texture.h"
#include "vertex_array.h"
#include <glm.hpp>

namespace arc {
struct OverlayVertex {
  glm::vec3 pos;
  glm::vec2 tex_coord;
};
class OverlayRenderer {

public:
  void Init();
  void Destroy();
  void Begin(Shader& shader);
  void End(Shader& shader);

private:
  uint indices_[6];
  OverlayVertex vertices_[4];
  VertexArray vertex_array_;
  IndexBuffer index_buffer_;
  VertexBuffer vertex_buffer_;
};

} // namespace arc

#endif // ARC_ENGINE_VOXEL_RENDERER_H
