// Created by adolin on 23. 12. 20.
//

#include "overlay_renderer.h"
#include "renderer_api.h"
#include <glm/gtc/matrix_transform.hpp>

#include <engine.h>
#include <glm/gtx/rotate_vector.hpp>
namespace arc {

void OverlayRenderer::Init() {
  vertex_array_.Setup();

  vertex_buffer_.Setup(4 * sizeof(OverlayVertex));
  vertex_buffer_.set_layout({
      {ShaderDataType::Float3, "a_position"},
      {ShaderDataType::Float2, "a_tex_coord"},
  });

  OverlayVertex vertices[4] = {
      {{-1, -1, 0}, {0, 0}}, 
      {{1, -1, 0}, {1, 0}},
      {{1, 1, 0}, {1, 1}},  
      {{-1, 1, 0}, {0, 1}},};
  uint indices[6] = {
      0, 1,2,2,3,0
  };
  vertex_buffer_.SetData(vertices, 4 * sizeof(OverlayVertex));
  index_buffer_.Setup(indices, 6);
  vertex_array_.BindIndexBuffer(index_buffer_);
  vertex_array_.BindVertexBuffer(vertex_buffer_);
  vertex_array_.Unbind();
  index_buffer_.Unbind();
  vertex_buffer_.Unbind();
}

void OverlayRenderer::Destroy() {}

void OverlayRenderer::Begin(Shader &shader) {
  shader.Bind();
  vertex_array_.Bind();
  index_buffer_.Bind();
  vertex_buffer_.Bind();
}

void OverlayRenderer::End(Shader &shader) {

  RendererAPI::DrawIndexed(vertex_array_, 6);
  index_buffer_.Unbind();
  vertex_buffer_.Unbind();
  vertex_array_.Unbind();
  shader.Unbind();
}

}
