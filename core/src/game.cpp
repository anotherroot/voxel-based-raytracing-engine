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
  /* arc::Engine::window().SetVsync(false); */
  scene_.Setup("Main Scene");

  {
    guy_model_.Setup("walterwhite.vox");
    int num = 1;
    float rad = 8;
    float delta = 360 / (float)num;

    for (int i = 0; i < num; ++i) {
      float angle = i * delta;
      float sin = glm::sin(glm::radians(angle));
      float cos = glm::sin(glm::radians(angle));
      auto guy = scene_.CreateEntity("Guy" + std::to_string(i));
      guy.Add<arc::ModelComponent>(&guy_model_);
      arc::Rotate(guy, -90, {1, 0, 0});
      /* arc::Rotate(guy, -90, {0, 0, 1}); */
      arc::Rotate(guy, angle, {0, 0, 1});
      arc::Translate(guy, {sin * rad, 0, cos * rad});
      walters_.push_back(guy);
    }
  }
  {
    /* auto guy = scene_.CreateEntity("center guy"); */
    /* guy.Add<arc::ModelComponent>(&guy_model_).voxel_size = {20, 0.1, 20}; */
    /* arc::Translate(guy, {0, -10, -5}); */
  }

  {
    /* auto floor = scene_.CreateEntity("Floor"); */
    /* floor.Add<arc::BoxComponent>(glm::vec3(1, 1, 1), glm::vec3(20, 0.5, 20));
     */
    /* arc::Translate(floor, {0, -3, 0}); */
  }

  {
    light_ = scene_.CreateEntity("light");
    auto &trans = light_.Get<arc::TransformComponent>().transform;
    glm::vec3 color = glm::vec3(1, 0.5, 0.5);
    arc::Translate(light_, {-5, 3, -2});
    light_.Add<arc::LightComponent>(20.0f, 0.0f, color, 0.1f);
  }
  {
    auto light_ = scene_.CreateEntity("light");
    auto &trans = light_.Get<arc::TransformComponent>().transform;
    glm::vec3 color = glm::vec3(0.5, 1, 1);
    arc::Translate(light_, {5, 5, -2});
    light_.Add<arc::LightComponent>(10.0f, 0.0f, color, 0.3f);
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
    float cos = glm::cos(arc::Engine::running_time() / 3.0);
    float sin = glm::sin(arc::Engine::running_time() / 3.0);
    /* arc::Rotate(walters_[0],arc::Engine::delta_time()*10, {1,0,0}); */
    /* arc::SetPosition(walters_[0], glm::vec3(5*cos,0,5*sin)); */
    arc::SetPosition(light_, glm::vec3(cos * 5, 1, sin * 5));
    /* arc::Rotate(walters_[0], arc::Engine::delta_time(), {0, 0, 1}); */
  }
  renderer_system_.Update(scene_);
}

void Game::ImGuiRender() {

  ImGui::Begin("fps");
  ImGui::Text(std::to_string(fps_).c_str());
  ImGui::End();
  if(1)
  {
    ImGui::Begin("First pass");
    ImGui::Text("albedo");
    ImGui::Image(reinterpret_cast<ImTextureID>(
                     renderer_system_.main_fb_.GetAttachment(0)),
                 ImVec2(arc::Engine::window().width() / 2,
                        arc::Engine::window().height() / 2),
                 ImVec2(0, 1), ImVec2(1, 0));

    ImGui::Text("normal");
    ImGui::Image(reinterpret_cast<ImTextureID>(
                     renderer_system_.main_fb_.GetAttachment(1)),
                 ImVec2(arc::Engine::window().width() / 2,
                        arc::Engine::window().height() / 2),
                 ImVec2(0, 1), ImVec2(1, 0));

    ImGui::Text("position");
    ImGui::Image(reinterpret_cast<ImTextureID>(
                     renderer_system_.main_fb_.GetAttachment(2)),
                 ImVec2(arc::Engine::window().width() / 2,
                        arc::Engine::window().height() / 2),
                 ImVec2(0, 1), ImVec2(1, 0));

    ImGui::Text("diffuse");
    ImGui::Image(reinterpret_cast<ImTextureID>(
                     renderer_system_.main_fb_.GetAttachment(3)),
                 ImVec2(arc::Engine::window().width() / 2,
                        arc::Engine::window().height() / 2),
                 ImVec2(0, 1), ImVec2(1, 0));



    ImGui::Text("debug");
    ImGui::Image(reinterpret_cast<ImTextureID>(
                     renderer_system_.main_fb_.GetAttachment(4)),
                 ImVec2(arc::Engine::window().width() / 2,
                        arc::Engine::window().height() / 2),
                 ImVec2(0, 1), ImVec2(1, 0));


    ImGui::End();

  }

  if(1)
  {
    ImGui::Begin("Soft Shadow filter");
    for (int i = 0; i < 3; ++i) {
      ImGui::Text(("fb "+std::to_string(i)).c_str());
      ImGui::Image(reinterpret_cast<ImTextureID>(
                       renderer_system_.soft_shadow_fb_[i].GetAttachment(0)),
                   ImVec2(arc::Engine::window().width() / 2,
                          arc::Engine::window().height() / 2),
                   ImVec2(0, 1), ImVec2(1, 0));
    }
    ImGui::End();
  }


  if(1)
  {
    ImGui::Begin("median filter");
    for (int i = 0; i < 3; ++i) {
      ImGui::Text(("fb "+std::to_string(i)).c_str());
      ImGui::Image(reinterpret_cast<ImTextureID>(
                       renderer_system_.median_fb_[i].GetAttachment(0)),
                   ImVec2(arc::Engine::window().width() / 2,
                          arc::Engine::window().height() / 2),
                   ImVec2(0, 1), ImVec2(1, 0));
    }
    ImGui::End();
  }
  ImGui::Begin("Options");
  ImGui::InputInt("Num Rays", &renderer_system_.opt_.AO_num_rays);
  ImGui::InputFloat("AO Ray dist", &renderer_system_.opt_.AO_ray_dist);
  ImGui::Text("Scene Light values");
  ImGui::ColorEdit3("color", (float *)&renderer_system_.opt_.ambient_color);
  ImGui::InputFloat("diffuse scalar", &renderer_system_.opt_.diffuse_scalar);
  ImGui::InputFloat("ambient scalar", &renderer_system_.opt_.ambient_scalar);
  ImGui::InputFloat("specular scalar", &renderer_system_.opt_.specular_scalar);
  ImGui::Text("Shadows");
  int i = renderer_system_.opt_.use_soft_shadow;
  ImGui::InputInt("Soft shadows", &i);
  renderer_system_.opt_.use_soft_shadow = i;
  ImGui::InputInt("Soft radius", &renderer_system_.opt_.ss_filter_radius);
  ImGui::InputInt("Soft passes", &renderer_system_.opt_.ss_num_passes);
  i = renderer_system_.opt_.ss_cross_type;
  ImGui::InputInt("cross filter", &i);
  renderer_system_.opt_.ss_cross_type = i;
  ImGui::InputInt("Num old", &renderer_system_.opt_.ss_num_old);
  ImGui::InputFloat("position treshold", &renderer_system_.opt_.ss_pos_th);
  ImGui::Text("Post processing");
  ImGui::InputInt("Median passes", &renderer_system_.opt_.median_num_passes);
  ImGui::InputInt("Median radius", &renderer_system_.opt_.median_radius);
  ImGui::InputInt("New median", &renderer_system_.opt_.new_median_shader);
  ImGui::InputInt("Change lum", &renderer_system_.opt_.change_ambient_based_on_color);
  ImGui::InputInt("dithering", &renderer_system_.opt_.dithering);
  i = renderer_system_.opt_.use_fsaa;
  ImGui::InputInt("FSAA", &i);
  renderer_system_.opt_.use_fsaa = i;
  ImGui::End();
}
