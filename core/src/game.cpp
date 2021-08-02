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
  {
    guy_ = scene_.CreateEntity("Guy");
    guy_model_.Setup("walterwhite.vox");
    auto mc = guy_.Add<arc::ModelComponent>(&guy_model_);
    auto &t = guy_.Get<arc::TransformComponent>();
    t.transform =
        glm::rotate(t.transform, glm::radians(-90.0f), glm::vec3(1, 0, 0));
  }
  {
    auto floor = scene_.CreateEntity("Floor");
    floor.Add<arc::BoxComponent>(glm::vec3(1,1,1),glm::vec3(20,0.5,20)); 
    auto &t = floor.Get<arc::TransformComponent>();
    t.transform = glm::translate(t.transform, {0,-3,0});
  }
  {

    auto light = scene_.CreateEntity("light");
    auto &trans = light.Get<arc::TransformComponent>().transform;
    trans = glm::translate(trans, {5, 7, 3});
    light.Add<arc::LightComponent>(100.0f, 0.0f, glm::vec3(1, 1, 1));
  }

  /* { */
  /*   auto light = scene_.CreateEntity("light2"); */
  /*   auto &trans = light.Get<arc::TransformComponent>().transform; */
  /*   trans = glm::translate(trans, {-5, 7, -3}); */
  /*   light.Add<arc::LightComponent>(100.0f, 0.0f, glm::vec3{1, 0.25, 0.25});
   */
  /* } */
  renderer_system_.Setup(scene_);
}

void Game::Dispose() {

  renderer_system_.Dispose();
  guy_model_.Dispose();
}

void Game::Update() { renderer_system_.Update(scene_); }

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
