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
  arc::Engine::window().SetVsync(false);
  scene_.Setup("Main Scene");

  {
      walter_ = models_.AddModel("walterwhite.vox");
      /* auto walter = models_.AddModel("Skins/skin-zombie-3.vox"); */
      auto entity = scene_.CreateEntity("walter");
      entity.Add<arc::ModelComponent>(walter_);
      arc::Translate(entity,{0,0,0});
      arc::Rotate(entity,-90,{1,0,0});
  }

  {
      /* std::vector<arc::Material> materials; */
      /* arc::Material mat; */
      /* materials.push_back({{1, 0.75, 0.5}, 0.5, 0, 0.5}); */
      /* materials.push_back({{1, 0.5, 1}, 0, 0.75, 0.5}); */
      /* materials.push_back({{0, 0.75, 0.5}, 0, 0.5, 0}); */
      /* const int w = 15, h = 15, d = 15; */
      /* ; */
      /* uint8_t data[w * h * d]{0}; */
      /* for (int i = 0; i < w; ++i) { */
      /*   for (int j = 0; j < h; ++j) { */
      /*     for (int k = 0; k < d; ++k) { */
      /*       data[i * h * d + j * d + k] = */
      /*           (i % 4 == 0) + (j % 4 == 0) + (k % 4 == 0); */
      /*     } */
      /*   } */
      /* } */
      /* auto model = models_.AddModel("Sphere", w, h, d, data, materials); */
      /* auto entity = scene_.CreateEntity("Sphere"); */
      /* entity.Add<arc::ModelComponent>(model); */
      /* arc::Translate(entity, {0, -5, 0}); */
      /* /1* arc::Rotate(entity,-90,{1,0,0}); *1/ */
  } 

  {
      std::vector<arc::Material> materials;
      arc::Material mat;
      mat.color = {1, 0.7, 0.75};
      mat.reflection = 1;
      mat.refraction = 0;
      mat.roughness = 0.0;

      materials.push_back(mat);
      mat.reflection = 0;
      materials.push_back(mat);
      const int w = 1, h = 128, d = 128;
      ;
      uint8_t data[w * h * d]{1};

      for (int i = 0; i < w; ++i) {
        for (int j = 0; j < h; ++j) {
          for (int k = 0; k < d; ++k) {
            data[k * w * h + j * w + i] = 1;
            if (j == 0 || k == 0 || j == h - 1 || k == d - 1)
              data[k * w * h + j * w + i] = 2;
          }
        }
      }

      auto model = models_.AddModel("mirror", w, h, d, data, materials);
      {
        auto entity = scene_.CreateEntity("mirror");
        entity.Add<arc::ModelComponent>(model);
        arc::Translate(entity, {3, 0, 0});
      }
      {
        auto entity = scene_.CreateEntity("mirror2");
        entity.Add<arc::ModelComponent>(model);
        arc::Translate(entity, {-3, 0, 0});
      }
  }

  {
    for (auto &mat : models_.materials()) {
      auto m = scene_.CreateEntity("u_materials[" +
                                   std::to_string(mat.second.index - 1) + "]");
      m.Add<arc::MaterialComponent>(mat.second.mat);
    }
  }

  {
    light_ = scene_.CreateEntity("light 1");
    auto &trans = light_.Get<arc::TransformComponent>().transform;
    glm::vec3 color = glm::vec3(1, 0.5, 0.5);
    arc::Translate(light_, {-5, 3, -2});
    light_.Add<arc::LightComponent>(200.0f, color, 0.1f);
  }
  {
    auto light_ = scene_.CreateEntity("light2");
    auto &trans = light_.Get<arc::TransformComponent>().transform;
    glm::vec3 color = glm::vec3(0.5, 1, 1);
    arc::Translate(light_, {5, 5, -2});
    light_.Add<arc::LightComponent>(100.0f, color, 0.3f);
  }
  renderer_system_.Setup(scene_);
}

void Game::Dispose() {
  renderer_system_.Dispose();
  models_.Dispose();
}

void Game::Update() {
  num_frames_ += 1;
  second_ += arc::Engine::delta_time();
  if (num_frames_ >= 100) {
    fps_ = num_frames_ / second_;
    logi("100 frames in {0}s, fps: {1}", second_, fps_);
    second_ = 0;
    num_frames_ = 0;
  }

  renderer_system_.Update(scene_);
}

void Game::ImGuiRender() {

  ImGui::Begin("Data");
  ImGui::Text(("fps: " + std::to_string(fps_)).c_str());
  ImGui::Text(
      ("old count: " + std::to_string(renderer_system_.num_old_)).c_str());

  ImGui::End();
  if (1) {
    ImGui::Begin("First pass ");

    ImGui::Text("Barva, Normale in Globina");
    ImGui::Image(reinterpret_cast<ImTextureID>(
                     renderer_system_.main_fb_.GetAttachment(0)),
                 ImVec2(arc::Engine::window().width() / 4,
                        arc::Engine::window().height() / 4),
                 ImVec2(0, 1), ImVec2(1, 0));
    ImGui::SameLine();

    ImGui::Image(reinterpret_cast<ImTextureID>(
                     renderer_system_.main_fb_.GetAttachment(1)),
                 ImVec2(arc::Engine::window().width() / 4,
                        arc::Engine::window().height() / 4),
                 ImVec2(0, 1), ImVec2(1, 0));
    ImGui::SameLine();
    ImGui::Image(reinterpret_cast<ImTextureID>(
                     renderer_system_.main_fb_.GetAttachment(7)),
                 ImVec2(arc::Engine::window().width() / 4,
                        arc::Engine::window().height() / 4),
                 ImVec2(0, 1), ImVec2(1, 0));

    ImGui::Text("Neposredna svetloba, Globalna svetloba in Refleksije");

    ImGui::Image(reinterpret_cast<ImTextureID>(
                     renderer_system_.main_fb_.GetAttachment(3)),
                 ImVec2(arc::Engine::window().width() / 4,
                        arc::Engine::window().height() / 4),
                 ImVec2(0, 1), ImVec2(1, 0));

    ImGui::SameLine();
    ImGui::Image(reinterpret_cast<ImTextureID>(
                     renderer_system_.main_fb_.GetAttachment(4)),
                 ImVec2(arc::Engine::window().width() / 4,
                        arc::Engine::window().height() / 4),
                 ImVec2(0, 1), ImVec2(1, 0));
    ImGui::SameLine();
    ImGui::Image(reinterpret_cast<ImTextureID>(
                     renderer_system_.main_fb_.GetAttachment(5)),
                 ImVec2(arc::Engine::window().width() / 4,
                        arc::Engine::window().height() / 4),
                 ImVec2(0, 1), ImVec2(1, 0));

    ImGui::Text("Neposredna svetloba in posredna svetloba");
    ImGui::Image(reinterpret_cast<ImTextureID>(
                     renderer_system_.old_fbs_[1].GetAttachment(0)),
                 ImVec2(arc::Engine::window().width() / 4,
                        arc::Engine::window().height() / 4),
                 ImVec2(0, 1), ImVec2(1, 0));
    ImGui::SameLine();
    ImGui::Image(reinterpret_cast<ImTextureID>(
                     renderer_system_.old_fbs_[1].GetAttachment(1)),
                 ImVec2(arc::Engine::window().width() / 4,
                        arc::Engine::window().height() / 4),
                 ImVec2(0, 1), ImVec2(1, 0));

    ImGui::Text("Refleksije in koncna slika");
    ImGui::Image(reinterpret_cast<ImTextureID>(
                     renderer_system_.old_fbs_[1].GetAttachment(2)),
                 ImVec2(arc::Engine::window().width() / 4,
                        arc::Engine::window().height() / 4),
                 ImVec2(0, 1), ImVec2(1, 0));
    ImGui::SameLine();
    ImGui::Image(reinterpret_cast<ImTextureID>(
                     renderer_system_.combine_fb_.GetAttachment(0)),
                 ImVec2(arc::Engine::window().width() / 4,
                        arc::Engine::window().height() / 4),
                 ImVec2(0, 1), ImVec2(1, 0));

    ImGui::Text("complex");
    ImGui::Image(reinterpret_cast<ImTextureID>(
                     renderer_system_.main_fb_.GetAttachment(6)),
                 ImVec2(arc::Engine::window().width() / 2,
                        arc::Engine::window().height() / 2),
                 ImVec2(0, 1), ImVec2(1, 0));

    ImGui::End();
  }
  ImGui::ShowDemoWindow();
  {
    ImGui::Begin("Models");
    auto models =
        scene_.registry()
            .view<const arc::TransformComponent, arc::ModelComponent>();
    for (auto entt : models) {
      arc::Entity entity(entt, &scene_);
      auto tag = entity.Get<arc::TagComponent>();
      auto &model = entity.Get<arc::ModelComponent>();
      auto &transform = entity.Get<arc::TransformComponent>();
      if (ImGui::TreeNode(tag.tag.c_str())) {
        if (ImGui::DragFloat3("Position", (float *)(&transform.transform[3][0]),
                              0.1, -10000, 10000, nullptr)) {
          model.changed = true;
        }
        if (ImGui::DragFloat3("Voxel size", (float *)(&model.voxel_size[0]),
                              0.1, 0.01, 100, nullptr)) {
          model.changed = true;
        }
        ImGui::TreePop();
        ImGui::Separator();
      }
    }
    ImGui::End();
  }
  {
    ImGui::Begin("Materials");
    auto models = scene_.registry().view<const arc::MaterialComponent>();
    for (auto entt : models) {
      arc::Entity entity(entt, &scene_);
      auto tag = entity.Get<arc::TagComponent>();
      auto &mat = entity.Get<arc::MaterialComponent>();
      if (ImGui::TreeNode(tag.tag.c_str())) {
        if (ImGui::ColorEdit3("Color", (float *)(&mat.mat.color[0]))) {
          mat.changed = true;
        }
        if (ImGui::DragFloat("Reflection", &mat.mat.reflection, 0.01, 0.0, 1.0,
                             nullptr)) {
          mat.changed = true;
        }
        if (ImGui::DragFloat("roughness", &mat.mat.roughness, 0.01, 0.0, 1.0,
                             nullptr)) {
          mat.changed = true;
        }

        ImGui::TreePop();
        ImGui::Separator();
      }
    }
    ImGui::End();
  }
  {
    ImGui::Begin("Lights");
    auto lights =
        scene_.registry()
            .view<const arc::TransformComponent, arc::LightComponent>();
    for (auto entt : lights) {
      arc::Entity entity(entt, &scene_);
      auto tag = entity.Get<arc::TagComponent>();
      auto &light = entity.Get<arc::LightComponent>();
      auto &transform = entity.Get<arc::TransformComponent>();
      if (ImGui::TreeNode(tag.tag.c_str())) {
        if (ImGui::DragFloat3("Position", (float *)(&transform.transform[3][0]),
                              0.1, -10000, 10000, nullptr)) {
          light.moded = true;
        }
        if (ImGui::ColorEdit3("Color", (float *)(&light.color[0]))) {
          light.moded = true;
        }
        if (ImGui::DragFloat("Radius", &light.radius, 0.1, 0.01, 100,
                             nullptr)) {
          light.moded = true;
        }
        if (ImGui::DragFloat("Range", &light.range, 0.1, 0.01, 100, nullptr)) {
          light.moded = true;
        }
        ImGui::TreePop();
        ImGui::Separator();
      }
    }
    ImGui::End();
  }

  /* ImGui::ShowDemoWindow(); */
  {

    ImGui::Begin("Options");
    {
      auto walters =
          scene_.registry()
              .view<const arc::TransformComponent, arc::ModelComponent>();
      int number = walters.size_hint();
      int row_size = 10;
      if (ImGui::InputInt("Number of walters", &number)) {
        for (auto walt : walters) {
          scene_.registry().destroy(walt);
        }
        for (int i = 0; i < number; ++i) {
            auto entity = scene_.CreateEntity("Walter");
            entity.Add<arc::ModelComponent>(walter_);
            arc::Translate(entity, {(i % row_size - row_size / 2.0) * 1.5,
                                    i / row_size, -i * 1.5 / row_size});
            arc::Rotate(entity, -90, {1, 0, 0});
        }
      }
    }
    ImGui::InputInt("Traversal Algo", &renderer_system_.opt_.traversal_algo);
    ImGui::InputInt("Reflection depth",
                    &renderer_system_.opt_.reflection_depth_);
    ImGui::ColorEdit3("Scene background",
                      (float *)(&renderer_system_.opt_.bacground_color[0]));
    ImGui::Text("Diffuse");

    ImGui::InputFloat("Diffuse scalar", &renderer_system_.opt_.diffuse_scalar);
    ImGui::InputInt("Soft shadows", &renderer_system_.opt_.use_soft_shadow);
    ImGui::InputInt("Diffuse filter", &renderer_system_.opt_.diffuse_filter);
    ImGui::InputInt("Diffuse Num passes",
                    &renderer_system_.opt_.diffuse_num_passes);
    ImGui::InputFloat("Diffuse position treshold",
                      &renderer_system_.opt_.diffuse_filter_position_treshold);
    ImGui::InputInt("Diffuse Radius",
                    &renderer_system_.opt_.diffuse_filter_radius);
    ImGui::InputInt("Diffuse Type", &renderer_system_.opt_.diffuse_filter_type);

    ImGui::Text("Ambient");
    ImGui::InputFloat("Ambient scalar", &renderer_system_.opt_.ambient_scalar);
    ImGui::InputFloat("Ambient start", &renderer_system_.opt_.ambient_start);
    ImGui::InputInt("Num rays", &renderer_system_.opt_.ambient_num_rays);
    ImGui::InputFloat("Ray distance",
                      &renderer_system_.opt_.ambient_ray_distance);
    ImGui::ColorEdit3("Color",
                      (float *)(&renderer_system_.opt_.ambient_color[0]));
    ImGui::InputInt("Ambient filter", &renderer_system_.opt_.ambient_filter);
    ImGui::InputInt("Ambient Num passes",
                    &renderer_system_.opt_.ambient_num_passes);
    ImGui::InputFloat("Ambient position treshold",
                      &renderer_system_.opt_.ambient_filter_position_treshold);
    ImGui::InputInt("Ambient Radius",
                    &renderer_system_.opt_.ambient_filter_radius);
    ImGui::InputInt("Ambient Type", &renderer_system_.opt_.ambient_filter_type);

    ImGui::Text("bilateral filter");
    ImGui::InputInt("Bilateral num passes",
                    &renderer_system_.opt_.num_bilateral_passes);
    ImGui::InputFloat("Bilateral treshold",
                      &renderer_system_.opt_.bilateral_treshold);
    ImGui::Text("postprocessing");
    ImGui::InputInt("Dithering", &renderer_system_.opt_.dithering);
    ImGui::InputInt("FSAA", &renderer_system_.opt_.use_fsaa);

    ImGui::End();
  }
  
  ImGui::Begin("Window size");
  int size[2] = {arc::Engine::window().width(),arc::Engine::window().height()};
  if(ImGui::InputInt2("size",size)){
    arc::Engine::window().SetSize(size[0],size[1]);
  }
  
}
