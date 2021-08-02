#include "renderer_system.h"
#include "components.h"
#include "engine.h"
#include "voxel_model.h"
void RendererSystem::Setup(arc::Scene &scene) {

  {
    arc::EventHandler::SubscribeWindow(this);
    uint wwidth = arc::Engine::window().width();
    uint wheight = arc::Engine::window().height();
    arc::FrameBufferConfig config(
        wwidth, wheight, 1,
        {{arc::TextureConfig::RGBA, arc::TextureConfig::BYTE,
          arc::TextureConfig::NEAREST}, // albedo
         {arc::TextureConfig::RGBA, arc::TextureConfig::BYTE,
          arc::TextureConfig::NEAREST}, // normal
         {arc::TextureConfig::RGBA, arc::TextureConfig::FLOAT,
          arc::TextureConfig::NEAREST}}, // position
        true);
    frame_buffer_.Setup(config);
  }

  voxel_shader_.Setup("/home/tilen/Projects/diploma/renderer/"
                      "engine/shaders/voxel_shader");
  shadow_shader_.Setup("/home/tilen/Projects/diploma/renderer/"
                       "engine/shaders/voxel_shadow_shader");
  renderer_.Init();
  shadow_texture_.Init(128, 128, 128, {0.2, 0.2, 0.2}, {});
  auto view = scene.registry().view<const arc::ModelComponent>();
  for (auto entity : view) {
    auto model = scene.registry().get<arc::ModelComponent>(entity);
    auto transform = scene.registry().get<arc::TransformComponent>(entity);
    shadow_texture_.SetBits(*model.model,transform.transform[3]);
  }
}
void RendererSystem::Dispose() { renderer_.Destroy(); }
void RendererSystem::Update(arc::Scene &scene) {
  camera_.Update(arc::Engine::delta_time());

  frame_buffer_.Bind();
  arc::RendererAPI::SetClearColor({0.25, 0.25, 0.75, 1});
  arc::RendererAPI::Clear();
  renderer_.Begin(voxel_shader_, camera_.camera());
  {
    {
      auto view =
          scene.registry()
              .view<const arc::TransformComponent, arc::ModelComponent>();
      for (auto [entity, transform, model] : view.each()) {
        // update shadow map
        if (transform.changed) {
          // TODO: update shadow map when things move
        }
        renderer_.DrawModel(voxel_shader_, model.model->texture(0),
                            model.model->materials(), transform.transform,
                            glm::vec3(0.1));
      }
    }
    {
      auto view = scene.registry()
                      .view<const arc::TransformComponent, arc::BoxComponent>();
      for (auto [entity, transform, box] : view.each()) {
        renderer_.DrawBox(voxel_shader_, transform.transform, box.size,
                          box.color);
      }
    }
  }
  renderer_.End(voxel_shader_);
  frame_buffer_.Unbind();
  arc::RendererAPI::SetClearColor({0.25, 0.25, 0.75, 1});
  arc::RendererAPI::Clear();
  renderer_.Begin(shadow_shader_, camera_.camera());
  {
    shadow_shader_.SetFloat2("u_window_size", {arc::Engine::window().width(),
                                               arc::Engine::window().height()});

    shadow_shader_.SetInt("u_albedo", 2);
    frame_buffer_.Bind(0, 2);
    shadow_shader_.SetInt("u_normal", 3);
    frame_buffer_.Bind(1, 3);
    shadow_shader_.SetInt("u_position", 4);
    frame_buffer_.Bind(2, 4);
    // lights
    {
      auto view =
          scene.registry()
              .view<const arc::TransformComponent, arc::LightComponent>();
      int i = 0;
      for (auto [entity, transform, light] : view.each()) {
        std::string str = "u_lights[" + std::to_string(i) + "].";
        shadow_shader_.SetFloat3(str + "position", transform.transform[3]);
        shadow_shader_.SetFloat(str + "long_range", light.long_range);
        shadow_shader_.SetFloat(str + "short_range", light.short_range);
        shadow_shader_.SetFloat3(str + "color", light.color);
        i++;
      }
      shadow_shader_.SetInt("u_num_lights", i);
    }

    renderer_.DrawTexture(shadow_shader_, shadow_texture_.texture(), {}, {},
                          glm::vec3(0.2));
  }
  renderer_.End(shadow_shader_);
}

bool RendererSystem::OnWindowResize(int width, int height) {
  frame_buffer_.Resize(width, height);
  return false;
}
