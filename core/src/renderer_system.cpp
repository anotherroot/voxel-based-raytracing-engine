#include "renderer_system.h"
#include "components.h"
#include "engine.h"
#include "glad/glad.h" // TODO: remove
#include "keys.h"
#include "voxel_model.h"
void RendererSystem::Setup(arc::Scene &scene) {

  arc::EventHandler::SubscribeWindow(this);
  arc::EventHandler::SubscribeKeyboard(this);
  {
    uint wwidth = arc::Engine::window().width();
    uint wheight = arc::Engine::window().height();
    arc::FrameBufferConfig config(wwidth, wheight, 1,
                                  {{arc::TexConf::RGBA, arc::TexConf::BYTE,
                                    arc::TexConf::NEAREST}, // albedo
                                   {arc::TexConf::RGBA, arc::TexConf::BYTE,
                                    arc::TexConf::NEAREST}, // normal
                                   {arc::TexConf::RGBA, arc::TexConf::FLOAT,
                                    arc::TexConf::NEAREST}}, // position
                                  true);
    frame_buffer_.Setup(config);
  }

  voxel_shader_.Setup("/home/tilen/Projects/diploma/renderer/"
                      "engine/shaders/voxel_shader");
  shadow_shader_.Setup("/home/tilen/Projects/diploma/renderer/"
                       "engine/shaders/voxel_shadow_shader");
  texture_shader_.Setup("/home/tilen/Projects/diploma/renderer/"
                        "engine/shaders/texture_shader");
  renderer_.Init();
  overlay_renderer_.Init();
  shadow_texture_.Init(512, 256, 512, {0.2, 0.2, 0.2}, {});
  /* blue_noise_tex_.Setup2D("LDR_RGB1_0.png"); */
  uint8_t data[16 * 16][3];
  for (int i = 0; i <16; ++i) {
    for (int j = 0; j < 16; ++j) {
      data[i * 16 + j][0] = 255 - i*4 - j*4;
      data[i * 16 + j][1] = i*4 + j*4;
      data[i * 16 + j][2] = 100 - i*4 + j*4;
    }
  }
  blue_noise_tex_.Setup2D(
      16, 16, data,
      {arc::TexConf::RGB, arc::TexConf::UNSIGNED_BYTE, arc::TexConf::NEAREST});
  SetShowShadowCaster(false);
}
void RendererSystem::Dispose() { renderer_.Destroy(); }
void RendererSystem::Update(arc::Scene &scene) {
  if (1) {
    frame_number_++;
    camera_.Update(arc::Engine::delta_time());
    { // set bits
      auto view = scene.registry().view<const arc::ModelComponent>();
      for (auto entity : view) {
        auto model = scene.registry().get<arc::ModelComponent>(entity);
        auto transform = scene.registry().get<arc::TransformComponent>(entity);
        shadow_texture_.SetBits(*model.model, transform.transform);
      }
    }

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
        auto view =
            scene.registry()
                .view<const arc::TransformComponent, arc::BoxComponent>();
        for (auto [entity, transform, box] : view.each()) {
          renderer_.DrawBox(voxel_shader_, transform.transform, box.size,
                            box.color);
        }
      }
    }
    renderer_.End(voxel_shader_);
    frame_buffer_.Unbind();
    if (1) {
      arc::RendererAPI::SetClearColor({0.25, 0.25, 0.75, 1});
      arc::RendererAPI::Clear();
      overlay_renderer_.Begin(shadow_shader_);
      {
        shadow_shader_.SetInt("u_frame_number", frame_number_);
        shadow_shader_.SetMat4("u_view_projection",
                               camera_.camera().view_projection());
        shadow_shader_.SetFloat3("u_origin",
                                 glm::inverse(camera_.camera().view())[3]);
        shadow_shader_.SetFloat2(
            "u_window_size",
            {arc::Engine::window().width(), arc::Engine::window().height()});

        shadow_shader_.SetInt("u_texture", 1);
        shadow_texture_.texture().Bind(1);
        shadow_shader_.SetInt("u_albedo", 2);
        frame_buffer_.Bind(0, 2);
        shadow_shader_.SetInt("u_normal", 3);
        frame_buffer_.Bind(1, 3);
        shadow_shader_.SetInt("u_blue_noise", 4);
        blue_noise_tex_.Bind(4);
        shadow_shader_.SetInt("u_position", 7);
        frame_buffer_.Bind(2, 7);
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

        // texture
        shadow_shader_.SetFloat3("u_texture_size",
                                 {shadow_texture_.texture().width(),
                                  shadow_texture_.texture().height(),
                                  shadow_texture_.texture().depth()});
        shadow_shader_.SetFloat3("u_box_position", {0, 0, 0});
        shadow_shader_.SetFloat3("u_box_size",
                                 {0.2f * shadow_texture_.texture().width(),
                                  0.2f * shadow_texture_.texture().height(),
                                  0.2f * shadow_texture_.texture().depth()});
      }
      overlay_renderer_.End(shadow_shader_);
    }

    { // clear bits
      auto view = scene.registry().view<const arc::ModelComponent>();
      for (auto entity : view) {
        auto model = scene.registry().get<arc::ModelComponent>(entity);
        auto transform = scene.registry().get<arc::TransformComponent>(entity);
        shadow_texture_.SetBits(*model.model, transform.transform, false);
      }
    }
  }
}

void RendererSystem::SetShowShadowCaster(bool value) {
  show_shadow_caster_ = value;
  shadow_shader_.Bind();
  shadow_shader_.SetInt("u_show_shadow_caster", (int)show_shadow_caster_);
  shadow_shader_.Unbind();
}
bool RendererSystem::OnWindowResize(int width, int height) {
  frame_buffer_.Resize(width, height);
  return false;
}

bool RendererSystem::OnKeyDown(int key, bool repeat) {
  if (!repeat && key == arc::Keys::T) {
    SetShowShadowCaster(!show_shadow_caster_);
  }

  return false;
}
