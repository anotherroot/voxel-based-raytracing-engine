// Created by adolin on 23. 12. 20.
//

#include "voxel_renderer.h"
#include "renderer_api.h"
#include <glm/gtc/matrix_transform.hpp>

#include <engine.h>
#include <glm/gtx/rotate_vector.hpp>
namespace arc {

void VoxelRenderer::Init() {
  vertex_array_.Setup();

  vertex_buffer_.Setup(8 * sizeof(VoxelVertex));
  vertex_buffer_.set_layout({
      {ShaderDataType::Float3, "a_position"},
      {ShaderDataType::Float3, "a_tex_coord"},
  });

  VoxelVertex vertices[8] = {
      {{-0.5, -0.5, -0.5}, {0, 0, 0}}, {{-0.5, -0.5, 0.5}, {0, 0, 1}},
      {{-0.5, 0.5, -0.5}, {0, 1, 0}},  {{-0.5, 0.5, 0.5}, {0, 1, 1}},
      {{0.5, -0.5, -0.5}, {1, 0, 0}},  {{0.5, -0.5, 0.5}, {1, 0, 1}},
      {{0.5, 0.5, -0.5}, {1, 1, 0}},   {{0.5, 0.5, 0.5}, {1, 1, 1}}};
  uint indices[36] = {
      0b001, 0b100, 0b000, 0b001, 0b101, 0b100, // bottom
      0b011, 0b010, 0b110, 0b011, 0b110, 0b111, // top
      0b000, 0b110, 0b010, 0b000, 0b100, 0b110, // back
      0b001, 0b011, 0b111, 0b001, 0b111, 0b101, // front
      0b000, 0b010, 0b011, 0b000, 0b011, 0b001, // left
      0b100, 0b111, 0b110, 0b100, 0b101, 0b111, // right
  };
  vertex_buffer_.SetData(vertices, 8 * sizeof(VoxelVertex));
  index_buffer_.Setup(indices, 36);
  vertex_array_.BindIndexBuffer(index_buffer_);
  vertex_array_.BindVertexBuffer(vertex_buffer_);
}

void VoxelRenderer::Destroy() {}

void VoxelRenderer::Begin(Shader &shader, const PerspectiveCamera &camera) {
  shader.Bind();
  shader.SetMat4("u_view_projection", camera.view_projection());
  shader.SetFloat3("u_origin", glm::inverse(camera.view())[3]);
  shader.SetInt("u_texture", 1);
  vertex_array_.Bind();
}

void VoxelRenderer::End(Shader &shader) { Flush(shader); }

void VoxelRenderer::Flush(Shader &shader) {

  vertex_array_.Unbind();
  shader.Unbind();
}

void VoxelRenderer::DrawTest(Shader &shader) {

  shader.Bind();
  shader.SetMat4("u_transform", glm::mat4(1));
  RendererAPI::DrawIndexed(vertex_array_, 36);
}

void VoxelRenderer::DrawModel(Shader &shader, const arc::Texture &texture,
                              const std::vector<Material> materials,
                              const glm::vec3 &pos, const glm::vec3 &size,
                              const glm::vec3 &rotation) {
  // transform
  glm::mat4 transform = glm::mat4(1);
  transform = glm::translate(transform, pos);
  for (int i = 0; i < 3; ++i) {
    transform = glm::rotate(transform, glm::radians(rotation[i]),
                            {i == 0, i == 1, i == 2});
  }
  transform = glm::scale(transform,
                         {size.x * texture.width(), size.y * texture.height(),
                          size.z * texture.depth()});
  shader.SetMat4("u_transform", transform);
  // materials
  std::string base = "u_materials[";
  std::string color = "].color";
  int i = 0;
  for (const Material &mat : materials) {
    std::string num = std::to_string(i++);
    shader.SetFloat3(base + num + color, mat.color);
  }
  // texture
  shader.SetFloat3("u_texture_size",
                   {texture.width(), texture.height(), texture.depth()});
  shader.SetFloat3("u_box_position", pos);
  shader.SetFloat3("u_box_size",
                   {size.x * texture.width(), size.y * texture.height(),
                    size.z * texture.depth()});
  texture.Bind(1);
  RendererAPI::DrawIndexed(vertex_array_, 36);
}
void VoxelRenderer::DrawModel(Shader &shader, const arc::Texture &texture,
                              const std::vector<Material> materials,
                              const glm::mat4 &trans,
                              const glm::vec3 &voxel_size) {
  // transform
  auto box_size = voxel_size *
                  glm::vec3{texture.width(), texture.height(), texture.depth()};
  auto transform = glm::scale(trans,box_size); 
  shader.SetMat4("u_transform", transform);
  // materials
  std::string base = "u_materials[";
  std::string color = "].color";
  int i = 0;
  for (const Material &mat : materials) {
    std::string num = std::to_string(i++);
    shader.SetFloat3(base + num + color, mat.color);
  }
  // texture
  shader.SetFloat3("u_texture_size",
                   {texture.width(), texture.height(), texture.depth()});
  shader.SetFloat3("u_box_position", trans[3]);
  shader.SetFloat3("u_box_size",box_size);
                  
  texture.Bind(1);
  RendererAPI::DrawIndexed(vertex_array_, 36);
}

void VoxelRenderer::SetMaterials(Shader &shader,
                                 const std::vector<Material> &materials) {
  // materials
  std::string base = "u_materials[";
  std::string color = "].color";
  int i = 0;
  for (const auto &mat : materials) {
    std::string num = std::to_string(i++);
    shader.SetFloat3(base + num + color, mat.color);
  }
}
void VoxelRenderer::DrawTextures(
    Shader &shader, const std::vector<Texture> &textures,
    const std::vector<VoxelTransform> &transforms) {

  for (int i = 0; i < textures.size(); ++i) {
    shader.SetMat4("u_transform", transforms[i].transform);
    // texture
    shader.SetFloat3(
        "u_texture_size",
        {textures[i].width(), textures[i].height(), textures[i].depth()});
    shader.SetFloat3("u_box_position", transforms[i].transform[3]);
    shader.SetFloat3("u_box_size", transforms[i].size);
    textures[i].Bind(1);
    RendererAPI::DrawIndexed(vertex_array_, 36);
  }
}
void VoxelRenderer::DrawTexture(Shader &shader, const Texture &texture,
                                const glm::vec3 &pos, const glm::vec3 &rot,
                                const glm::vec3 &size) {
  // transform
  glm::mat4 transform = glm::mat4(1);
  transform = glm::translate(transform, pos);
  for (int i = 0; i < 3; ++i) {
    transform =
        glm::rotate(transform, glm::radians(rot[i]), {i == 0, i == 1, i == 2});
  }
  transform = glm::scale(transform,
                         {size.x * texture.width(), size.y * texture.height(),
                          size.z * texture.depth()});
  shader.SetMat4("u_transform", transform);

  // texture
  shader.SetFloat3("u_texture_size",
                   {texture.width(), texture.height(), texture.depth()});
  shader.SetFloat3("u_box_position", pos);
  shader.SetFloat3("u_box_size",
                   {size.x * texture.width(), size.y * texture.height(),
                    size.z * texture.depth()});
  texture.Bind(1);
  RendererAPI::DrawIndexed(vertex_array_, 36);
}
} // namespace arc
