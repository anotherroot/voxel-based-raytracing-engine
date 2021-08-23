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
/* #include "imgui.h" */
void Game::Init() {
  /* arc::Engine::window().SetVsync(false); */
  scene_.Setup("Main Scene");

  {
    guy_model_.Setup("Top/top-long\ white\ shirt.vox");
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
    int i=0;
    for(auto &mat: guy_model_.materials()){
      auto m = scene_.CreateEntity("u_materials["+std::to_string(i)+"]");
      m.Add<arc::MaterialComponent>(mat);  
      i++;
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
                       renderer_system_.ambient_fb_[i].GetAttachment(0)),
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
                       renderer_system_.diffuse_fb_[i].GetAttachment(0)),
                   ImVec2(arc::Engine::window().width() / 2,
                          arc::Engine::window().height() / 2),
                   ImVec2(0, 1), ImVec2(1, 0));
    }
    ImGui::End();
  }
  /* ImGui::ShowDemoWindow(); */
  ImGui::Begin("Options");
  
  ImGui::ColorEdit3("Scene background", (float*)(&renderer_system_.opt_.bacground_color[0]));
  ImGui::Text("Diffuse");

  ImGui::InputFloat("Diffuse scalar", &renderer_system_.opt_.diffuse_scalar);
  ImGui::InputInt("Soft shadows", &renderer_system_.opt_.use_soft_shadow);
  ImGui::InputInt("Diffuse filter", &renderer_system_.opt_.diffuse_filter);
  ImGui::InputInt("Num passes", &renderer_system_.opt_.diffuse_num_passes);
  ImGui::InputFloat("Diff position treshold", &renderer_system_.opt_.diffuse_filter_position_treshold);
  ImGui::InputInt("Radius", &renderer_system_.opt_.diffuse_filter_radius);
  ImGui::InputInt("Type", &renderer_system_.opt_.diffuse_filter_type);

  ImGui::Text("Ambient");
  ImGui::InputFloat("Ambient scalar", &renderer_system_.opt_.ambient_scalar);
  ImGui::InputInt("Num rays", &renderer_system_.opt_.ambient_num_rays);
  ImGui::InputFloat("Ray distance", &renderer_system_.opt_.ambient_ray_distance);
  ImGui::ColorEdit3("Color", (float*)(&renderer_system_.opt_.ambient_color[0]));
  ImGui::InputInt("Based on color", &renderer_system_.opt_.ambient_based_on_color);
  ImGui::InputInt("Ambient filter", &renderer_system_.opt_.ambient_filter);
  ImGui::InputInt("Num passes", &renderer_system_.opt_.ambient_num_passes);
  ImGui::InputFloat("Ambi position treshold", &renderer_system_.opt_.ambient_filter_position_treshold);
  ImGui::InputInt("Radius", &renderer_system_.opt_.ambient_filter_radius);
  ImGui::InputInt("Type", &renderer_system_.opt_.ambient_filter_type);


  ImGui::Text("postprocessing");
  ImGui::InputInt("Temporal num", &renderer_system_.opt_.temporal_num);
  ImGui::InputInt("Dithering", &renderer_system_.opt_.dithering);
  ImGui::InputInt("FSAA", &renderer_system_.opt_.use_fsaa);

  ImGui::End();

  ImGui::Begin("Materials");

  auto materials =
      scene_.registry()
          .view<const  arc::MaterialComponent, arc::TagComponent>();
  for (auto [entity, material, tag] : materials.each()) {
    ImGui::ColorEdit3(tag.tag.c_str(), (float*)&(material.mat.color[0]));
  }

  
  ImGui::End();
}
