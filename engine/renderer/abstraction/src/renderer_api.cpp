//
// Created by adolin on 21. 12. 20.
//

#include "renderer_api.h"

#include <glad/glad.h>
namespace arc {
bool RendererAPI::setup_ = false;
void RendererAPI::Setup() {
  arc_assert(!setup_, "RendererAPI Setup failed! RendererAPI already exists");
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_DEPTH_TEST);
  /* glEnable(GL_MULTISAMPLE); */
  glEnable(GL_CULL_FACE);
  /* glEnable(GL_BACK); */
  glEnable(GL_TEXTURE_3D);

  logi("data: {0}", GL_MAX_VERTEX_UNIFORM_COMPONENTS);
}
void RendererAPI::Dispose() {
  if (setup_) {
    setup_ = false;
    logi("TODO: any cleanup?");
  }
}

void RendererAPI::SetViewport(uint x, uint y, uint width, uint height) {
  glViewport(x, y, width, height);
}

void RendererAPI::SetClearColor(const glm::vec4 &color) {
  glClearColor(color.r, color.g, color.b, color.a);
}

void RendererAPI::Clear() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void RendererAPI::DrawIndexed(const VertexArray &vertex_array, uint count) {
  glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
}

} // namespace arc
