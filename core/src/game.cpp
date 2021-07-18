#include "../include/game.h"
#include "engine.h"
#include "functions.h"
#include "glad/glad.h"
#include "imgui.h"
#include "renderer_api.h"
#include "structs.h"
#include <glm/gtc/matrix_transform.hpp>
Game::Game() {}

void Game::Init() {
  {
    arc::EventHandler::SubscribeWindow(this);
    uint wwidth = arc::Engine::window().width();
    uint wheight = arc::Engine::window().height();
    arc::FrameBufferConfig config(
        wwidth, wheight, 1,
        {{arc::TextureConfig::RGBA, arc::TextureConfig::BYTE,
          arc::TextureConfig::NEAREST},
         {arc::TextureConfig::RGBA, arc::TextureConfig::BYTE,
          arc::TextureConfig::NEAREST},
         {arc::TextureConfig::RGBA, arc::TextureConfig::FLOAT,
          arc::TextureConfig::NEAREST}},
        true);
    frame_buffer_.Setup(config);
  }
  voxel_shader_.Create("/home/tilen/Projects/diploma/impl/renderer/arc_engine/"
                       "engine/shaders/voxel_shader");
  shadow_shader_.Create("/home/tilen/Projects/diploma/impl/renderer/arc_engine/"
                        "engine/shaders/voxel_shadow_shader");
  renderer_.Init();
  model_.Setup("walterwhite.vox");
  uint width = 256, height = 16, depth = 256;
  uint8_t *data = new uint8_t[width * height * depth];
  materials_.push_back({arc::Color::FromHex(0xaa5500ff)});
  materials_.push_back({arc::Color::FromHex(0x66aa55ff)});
  for (int i = 0; i < width; ++i) {
    for (int j = 0; j < depth; ++j) {
      int h = 7;
      for (int k = 0; k < height; ++k) {
        uint8_t mat = k < h ? 1 : k == h ? 2 : 0;
        data[i * height * depth + k * depth + j] = mat;
      }
    }
  }
  textures_.push_back({});
  arc::TextureConfig config = {arc::TextureConfig::R,
                               arc::TextureConfig::UNSIGNED_BYTE,
                               arc::TextureConfig::NEAREST};
  textures_.back().Create3D(width, height, depth, data, config);
  transforms_.push_back(
      {glm::mat4(1), {0.1f * width, 0.1f * height, 0.1f * depth}});
  transforms_.back().transform =
      glm::scale(transforms_.back().transform, transforms_.back().size);
  transforms_.back().transform =
      glm::translate(transforms_.back().transform, {0, -10, 0});
  shadow_texture_.Init(256, 256, 256, {0.2, 0.2, 0.2}, {});
  shadow_texture_.SetBits(model_, {0,-0.5,2});
}

void Game::Dispose() { renderer_.Destroy(); }

void Game::Update() {

  camera_.Update(arc::Engine::delta_time());

  frame_buffer_.Bind();
  arc::RenderCommand::SetClearColor({0.25, 0.25, 0.75, 1});
  arc::RenderCommand::Clear();

  renderer_.Begin(voxel_shader_, camera_.camera());
  {
    renderer_.SetMaterials(voxel_shader_, materials_);
    renderer_.DrawTextures(voxel_shader_, textures_, transforms_);
    renderer_.DrawModel(voxel_shader_, model_.texture(0), model_.materials(),
                        {1+glm::sin(arc::Engine::running_time()),-0.5,2}, {0.1, 0.1, 0.1}, {0, 0, 0});
    renderer_.DrawModel(voxel_shader_, model_.texture(0), model_.materials(),
                        {glm::sin(arc::Engine::running_time()),2.01,2}, {0.1, 0.1, 0.1}, {90, 0, 0});
  }

  renderer_.End(voxel_shader_);
  frame_buffer_.Unbind();

  arc::RenderCommand::SetClearColor({0.25, 0.25, 0.75, 1});
  arc::RenderCommand::Clear();
  renderer_.Begin(shadow_shader_, camera_.camera());
  {
    shadow_shader_.SetFloat2("u_window_size", {arc::Engine::window().width(),
                                               arc::Engine::window().height()});
    shadow_shader_.SetInt("u_texture", 1);
    shadow_texture_.texture().Bind(1);
    shadow_shader_.SetInt("u_albedo", 2);
    frame_buffer_.Bind(0, 2);
    shadow_shader_.SetInt("u_normal", 3);
    frame_buffer_.Bind(1, 3);
    shadow_shader_.SetInt("u_position", 4);
    frame_buffer_.Bind(2, 4);
    //lights
    {
      shadow_shader_.SetInt("u_num_lights",1);
      shadow_shader_.SetFloat3("u_lights[0].position",{0,5,0});
      shadow_shader_.SetFloat("u_lights[0].short_range",100);
      shadow_shader_.SetFloat("u_lights[0].long_range",100);
      shadow_shader_.SetFloat3("u_lights[0].color",{1,1,1});
    }
    renderer_.DrawTexture(shadow_shader_, shadow_texture_.texture(), {}, {},
                          shadow_texture_.size());
  }
  renderer_.End(shadow_shader_);
}

void Game::ImGuiRender() {
  ImGui::Begin("frame_buffers");
  int i = 0;
  ImGui::Text("albedo");
  ImGui::Image(reinterpret_cast<ImTextureID>(frame_buffer_.GetAttachment(i++)),
               ImVec2(arc::Engine::window().width() / 2,
                      arc::Engine::window().height() / 2),
               ImVec2(0, 1), ImVec2(1, 0));
  ImGui::Text("normal");
  ImGui::Image(reinterpret_cast<ImTextureID>(frame_buffer_.GetAttachment(i++)),
               ImVec2(arc::Engine::window().width() / 2,
                      arc::Engine::window().height() / 2),
               ImVec2(0, 1), ImVec2(1, 0));
  ImGui::Text("position");
  ImGui::Image(reinterpret_cast<ImTextureID>(frame_buffer_.GetAttachment(i++)),
               ImVec2(arc::Engine::window().width() / 2,
                      arc::Engine::window().height() / 2),
               ImVec2(0, 1), ImVec2(1, 0));
  ImGui::Text("depth");
  ImGui::Image(reinterpret_cast<ImTextureID>(frame_buffer_.GetAttachment(i)),
               ImVec2(500, 500), ImVec2(0, 1), ImVec2(1, 0));
  ImGui::End();
}
bool Game::OnWindowResize(int width, int height) {
  frame_buffer_.Resize(width, height);
  return false;
}
