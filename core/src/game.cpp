#include "../include/game.h"
#include "components.h"
#include "engine.h"
#include "entity.h"
#include "func.h"
#include "functions.h"
#include "glad/glad.h"
#include "imgui.h"
#include "renderer_api.h"
#include "string"
#include "structs.h"
void Game::Init() {
  arc::Engine::window().SetVsync(false);
  scene_.Setup("Main Scene");

  {
    guy_model_.Setup("walterwhite.vox");
    for (int i = 0; i < 1; ++i) {
      for (int j = 0; j < 1; ++j) {
        auto guy = scene_.CreateEntity("Guy" + std::to_string(i));
        guy.Add<arc::ModelComponent>(&guy_model_);
        arc::Rotate(guy, -90, {1, 0, 0});
        arc::Translate(guy, {i*3 , j*3 , 0});
        walters_.push_back(guy);
      }
    }
  }

  {
    auto floor = scene_.CreateEntity("Floor");
    floor.Add<arc::BoxComponent>(glm::vec3(1, 1, 1), glm::vec3(20, 0.5, 20));
    arc::Translate(floor, {0, -3, 0});
  }
  {

    light_ = scene_.CreateEntity("light");
    auto &trans = light_.Get<arc::TransformComponent>().transform;
    light_.Add<arc::BoxComponent>(glm::vec3(1, 0, 0), glm::vec3(0.3));
    arc::Translate(light_, {-5, 10, -2});
    light_.Add<arc::LightComponent>(100.0f, 0.0f, glm::vec3(1, 1, 1),0.5f);
  }

  renderer_system_.Setup(scene_);
}

void Game::Dispose() {

  renderer_system_.Dispose();
  guy_model_.Dispose();
}

void Game::Update() {

  num_frames_ += 1;
  second_ += arc::Engine::delta_time();
  if (second_ >= 2) {
    second_ = 0;
    fps_ = num_frames_ / 2;
    num_frames_ = 0;
  }

  {
    float cos = glm::cos(arc::Engine::running_time() );
    float sin = glm::sin(arc::Engine::running_time() );
    /* arc::SetPosition( */
    /*     light_, glm::vec3(cos*5, 8,sin*5)); */
  }
  renderer_system_.Update(scene_);
}

void Game::ImGuiRender() {

  ImGui::Begin("fps");
  ImGui::Text(std::to_string(fps_).c_str());
  ImGui::End();

  int i = 0;
  ImGui::Text("albedo");
  ImGui::Image(reinterpret_cast<ImTextureID>(
                   renderer_system_.frame_buffer_.GetAttachment(i++)),
               ImVec2(arc::Engine::window().width() / 2,
                      arc::Engine::window().height() / 2),
               ImVec2(0, 1), ImVec2(1, 0));
  ImGui::Text("normal");
  ImGui::Image(reinterpret_cast<ImTextureID>(
                   renderer_system_.frame_buffer_.GetAttachment(i++)),
               ImVec2(arc::Engine::window().width() / 2,
                      arc::Engine::window().height() / 2),
               ImVec2(0, 1), ImVec2(1, 0));
  ImGui::Text("position");
  ImGui::Image(reinterpret_cast<ImTextureID>(
                   renderer_system_.frame_buffer_.GetAttachment(i++)),
               ImVec2(arc::Engine::window().width() / 2,
                      arc::Engine::window().height() / 2),
               ImVec2(0, 1), ImVec2(1, 0));

  ImGui::Text("position");
  ImGui::Image(reinterpret_cast<ImTextureID>(
                   renderer_system_.frame_buffer_.GetAttachment(i++)),
               ImVec2(arc::Engine::window().width() / 2,
                      arc::Engine::window().height() / 2),
               ImVec2(0, 1), ImVec2(1, 0));
  ImGui::Text("direct");
  ImGui::Image(reinterpret_cast<ImTextureID>(
                   renderer_system_.shadow_fb_.GetAttachment(0)),
               ImVec2(arc::Engine::window().width() / 2,
                      arc::Engine::window().height() / 2),
               ImVec2(0, 1), ImVec2(1, 0));
  ImGui::Text("ambient");
  ImGui::Image(reinterpret_cast<ImTextureID>(
                   renderer_system_.shadow_fb_.GetAttachment(1)),
               ImVec2(arc::Engine::window().width() / 2,
                      arc::Engine::window().height() / 2),
               ImVec2(0, 1), ImVec2(1, 0));

  ImGui::Text("ambient combined");
  ImGui::Image(reinterpret_cast<ImTextureID>(
                   renderer_system_.single_fb_[0].GetAttachment(0)),
               ImVec2(arc::Engine::window().width() / 2,
                      arc::Engine::window().height() / 2),
               ImVec2(0, 1), ImVec2(1, 0));

  ImGui::Text("ambient combined gauss");
  ImGui::Image(reinterpret_cast<ImTextureID>(
                   renderer_system_.single_fb_[1].GetAttachment(0)),
               ImVec2(arc::Engine::window().width() / 2,
                      arc::Engine::window().height() / 2),
               ImVec2(0, 1), ImVec2(1, 0));
  ImGui::End();

  ImGui::Begin("Options");
  ImGui::InputFloat("Bilateral tolerance",&renderer_system_.opt_.AO_bilateral_tolerance);
  ImGui::InputInt("Num Rays",&renderer_system_.opt_.AO_num_rays);
  if(ImGui::Selectable("With Bilateral",&renderer_system_.opt_.AO_with_bilateral_)){
    renderer_system_.opt_.AO_with_bilateral_ = !renderer_system_.opt_.AO_with_bilateral_;
  }
  ImGui::InputFloat("AO Ray dist",&renderer_system_.opt_.AO_ray_dist);
  ImGui::InputInt("AO max temporal",&renderer_system_.opt_.max_temporal);
  ImGui::Text("Scene Light values");
  ImGui::ColorEdit3("color",(float*)&renderer_system_.opt_.ambient_color);
  ImGui::InputFloat("diffuse scalar", &renderer_system_.opt_.diffuse_scalar);
  ImGui::InputFloat("ambient scalar", &renderer_system_.opt_.ambient_scalar);
  ImGui::InputFloat("specular scalar", &renderer_system_.opt_.specular_scalar);
  ImGui::End();
}
