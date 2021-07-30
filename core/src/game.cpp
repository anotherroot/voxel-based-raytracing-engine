#include "../include/game.h"
#include "components.h"
#include "engine.h"
#include "entity.h"
#include "functions.h"
#include "glad/glad.h"
#include "imgui.h"
#include "renderer_api.h"
#include "structs.h"
#include <glm/gtc/matrix_transform.hpp>

void Game::Init() {
  scene_.Setup("Main Scene");
  guy_ = scene_.CreateEntity("Guy");
  guy_model_.Setup("walterwhite.vox");
  auto mc = guy_.Add<arc::ModelComponent>(&guy_model_);
  {
    auto light = scene_.CreateEntity("light");
    auto &trans = light.Get<arc::TransformComponent>().transform;
    trans = glm::translate(trans, {5, 7, 3});
    light.Add<arc::LightComponent>(100.0f, 0.0f, glm::vec3(0.25, 0.25, 1));
  }
  {
    auto light = scene_.CreateEntity("light2");
    auto &trans = light.Get<arc::TransformComponent>().transform;
    trans = glm::translate(trans, {-5, 7, -3});
    light.Add<arc::LightComponent>(100.0f, 0.0f, glm::vec3{1, 0.25, 0.25});
  }
  renderer_system_.Setup();
}

void Game::Dispose() {


  renderer_system_.Dispose();
  guy_model_.Dispose();
}

void Game::Update() { 

  guy_.Get<arc::TransformComponent>().transform[3] =
      glm::vec4{5*glm::sin(arc::Engine::running_time()), -1,
                5*glm::cos(arc::Engine::running_time()), 1};
  renderer_system_.Update(scene_); }

void Game::ImGuiRender() {

  /* ImGui::Begin("frame_buffers"); */
  /* int i = 0; */
  /* ImGui::Text("albedo"); */
  /* ImGui::Image(reinterpret_cast<ImTextureID>(frame_buffer_.GetAttachment(i++)),
   */
  /*              ImVec2(arc::Engine::window().width() / 2, */
  /*                     arc::Engine::window().height() / 2), */
  /*              ImVec2(0, 1), ImVec2(1, 0)); */
  /* ImGui::Text("normal"); */
  /* ImGui::Image(reinterpret_cast<ImTextureID>(frame_buffer_.GetAttachment(i++)),
   */
  /*              ImVec2(arc::Engine::window().width() / 2, */
  /*                     arc::Engine::window().height() / 2), */
  /*              ImVec2(0, 1), ImVec2(1, 0)); */
  /* ImGui::Text("position"); */
  /* ImGui::Image(reinterpret_cast<ImTextureID>(frame_buffer_.GetAttachment(i++)),
   */
  /*              ImVec2(arc::Engine::window().width() / 2, */
  /*                     arc::Engine::window().height() / 2), */
  /*              ImVec2(0, 1), ImVec2(1, 0)); */
  /* ImGui::Text("depth"); */
  /* ImGui::Image(reinterpret_cast<ImTextureID>(frame_buffer_.GetAttachment(i)),
   */
  /*              ImVec2(500, 500), ImVec2(0, 1), ImVec2(1, 0)); */
  /* ImGui::End(); */
}
