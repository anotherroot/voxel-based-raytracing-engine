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
                                    arc::TexConf::NEAREST}, // position
                                   {arc::TexConf::RGBA, arc::TexConf::FLOAT,
                                    arc::TexConf::NEAREST}}, // OtherData
                                  true);
    frame_buffer_.Setup(config);

    shadow_fb_.Setup(
        {wwidth,
         wheight,
         1,
         {
             {arc::TexConf::RGBA, arc::TexConf::BYTE,
              arc::TexConf::NEAREST}, // light // already based on the material
             {arc::TexConf::RGBA, arc::TexConf::BYTE,
              arc::TexConf::NEAREST}, // shadow for debug
                                      // TODO: how will specular light work in
                                      // this cas
         },
         false});
    for (int i = 0; i < 2; ++i)
      single_fb_[i].Setup({wwidth,
                           wheight,
                           1,
                           {
                               {arc::TexConf::RGBA, arc::TexConf::BYTE,
                                arc::TexConf::NEAREST}, // whatever
                           },
                           false});
  }

  voxel_shader_.Setup("/home/tilen/Projects/diploma/renderer/"
                      "engine/shaders/voxel_shader");
  shadow_shader_.Setup("/home/tilen/Projects/diploma/renderer/"
                       "engine/shaders/voxel_shadow_shader");
  gauss_shader_.Setup("/home/tilen/Projects/diploma/renderer/"
                      "engine/shaders/gauss_shader");
  blurr_shader_.Setup("/home/tilen/Projects/diploma/renderer/"
                      "engine/shaders/blurr_shader");
  combine_shader_.Setup("/home/tilen/Projects/diploma/renderer/"
                        "engine/shaders/combine_shader");
  renderer_.Init();
  overlay_renderer_.Init();
  shadow_texture_.Init(512 * 4, 256 * 2, 512 * 4, {0.2, 0.2, 0.2}, {});
  blue_noise_tex_.Setup2D("LDR_RGB1_0.png", {});
  SetShowShadowCaster(false);
}
void RendererSystem::Dispose() { renderer_.Destroy(); }
void RendererSystem::Update(arc::Scene &scene) {
  if (1) {
    frame_number_++;
    camera_.Update(arc::Engine::delta_time());
    if (1) { // set bits

      auto view = scene.registry().view<const arc::ModelComponent>();
      for (auto entity : view) {
        auto &model = scene.registry().get<arc::ModelComponent>(entity);
        if (model.draw) {
          auto transform =
              scene.registry().get<arc::TransformComponent>(entity);
          shadow_texture_.SetBits(*model.model, transform.transform);
          model.draw = false;
        }
      }
    }

    frame_buffer_.Bind();
    arc::RendererAPI::SetClearColor({0.25, 0.25, 0.75, 0});
    arc::RendererAPI::Clear();
    renderer_.Begin(voxel_shader_, camera_.camera());
    {
      {
        auto view =
            scene.registry()
                .view<const arc::TransformComponent, arc::ModelComponent>();
        auto lights =
            scene.registry()
                .view<const arc::TransformComponent, arc::LightComponent>();
        for (auto [entity, transform, model] : view.each()) {
          // lights
          {
            int i = 0;
            for (auto [entity, lt, light] : lights.each()) {
              std::string str = "u_lights[" + std::to_string(i) + "].";
              glm::vec4 pos = lt.transform[3];
              voxel_shader_.SetFloat3(str + "position", pos);
              voxel_shader_.SetFloat(str + "long_range", light.long_range);
              voxel_shader_.SetFloat(str + "short_range", light.short_range);
              voxel_shader_.SetFloat3(str + "color", light.color);
              i++;
            }
            voxel_shader_.SetInt("u_num_lights", i);
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
      shadow_fb_.Bind();
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
        shadow_texture_.Bind(1);
        shadow_shader_.SetInt("u_albedo", 2);
        frame_buffer_.Bind(0, 2);
        shadow_shader_.SetInt("u_normal", 3);
        frame_buffer_.Bind(1, 3);
        shadow_shader_.SetInt("u_position", 4);
        frame_buffer_.Bind(2, 4);
        shadow_shader_.SetInt("u_buffer", 5);
        frame_buffer_.Bind(3, 5);
        shadow_shader_.SetInt("u_depth", 6);
        frame_buffer_.Bind(4, 6);
        shadow_shader_.SetInt("u_shadow", 6);
        single_fb_[0].Bind(0, 6);
        shadow_shader_.SetInt("u_blue_noise", 8);
        blue_noise_tex_.Bind(8);
        shadow_shader_.SetFloat2(
            "u_blue_noise_size",
            {blue_noise_tex_.width(), blue_noise_tex_.height()});
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
      shadow_fb_.Unbind();
    }

    if (1) { // gaussian blurr
      single_fb_[1].Bind();
      arc::RendererAPI::SetClearColor({0.25, 0.25, 0.75, 1});
      arc::RendererAPI::Clear();
      overlay_renderer_.Begin(gauss_shader_);
      {
        gauss_shader_.SetFloat2(
            "u_window_size",
            {arc::Engine::window().width(), arc::Engine::window().height()});
        gauss_shader_.SetInt("u_texture", 1);
        shadow_fb_.Bind(1, 1);
      }
      overlay_renderer_.End(gauss_shader_);
      single_fb_[1].Unbind();

      single_fb_[0].Bind();
      arc::RendererAPI::SetClearColor({0.25, 0.25, 0.75, 1});
      arc::RendererAPI::Clear();
      overlay_renderer_.Begin(gauss_shader_);
      {
        gauss_shader_.SetFloat2(
            "u_window_size",
            {arc::Engine::window().width(), arc::Engine::window().height()});
        gauss_shader_.SetInt("u_texture", 1);
        single_fb_[1].Bind(0, 1);
      }
      overlay_renderer_.End(gauss_shader_);
      single_fb_[0].Unbind();
    }

    if (0) { // normal blurr
      single_fb_[1].Bind();
      arc::RendererAPI::SetClearColor({0.25, 0.25, 0.75, 1});
      arc::RendererAPI::Clear();
      overlay_renderer_.Begin(blurr_shader_);
      {
        blurr_shader_.SetFloat2(
            "u_window_size",
            {arc::Engine::window().width(), arc::Engine::window().height()});
        blurr_shader_.SetInt("u_texture", 1);
        shadow_fb_.Bind(0, 1);
      }
      overlay_renderer_.End(blurr_shader_);
      single_fb_[1].Unbind();
      single_fb_[0].Bind();
      arc::RendererAPI::SetClearColor({0.25, 0.25, 0.75, 1});
      arc::RendererAPI::Clear();
      overlay_renderer_.Begin(blurr_shader_);
      {
        blurr_shader_.SetFloat2(
            "u_window_size",
            {arc::Engine::window().width(), arc::Engine::window().height()});
        blurr_shader_.SetInt("u_texture", 1);
        single_fb_[1].Bind(0, 1);
      }
      overlay_renderer_.End(blurr_shader_);
      single_fb_[0].Unbind();
    }
    if (1) // combine
    {
      arc::RendererAPI::SetClearColor({0.25, 0.25, 0.75, 1});
      arc::RendererAPI::Clear();
      overlay_renderer_.Begin(combine_shader_);
      {
        combine_shader_.SetInt("u_albedo", 1);
        frame_buffer_.Bind(0, 1);
        combine_shader_.SetInt("u_direct", 2);
        shadow_fb_.Bind(0, 2);
        combine_shader_.SetInt("u_ambient", 3);
        shadow_fb_.Bind(1, 3);
        /* single_fb_[0].Bind(0, 3); */
      }
      overlay_renderer_.End(combine_shader_);
    }
    if (1) { // clear bits
      auto view = scene.registry().view<const arc::ModelComponent>();
      for (auto entity : view) {
        auto &model = scene.registry().get<arc::ModelComponent>(entity);
        if (model.erase || model.draw) {
          auto &transform =
              scene.registry().get<arc::TransformComponent>(entity);
          shadow_texture_.SetBits(*model.model, transform.transform, false);
          model.erase = false;
        }
      }
    }
  }
}

void RendererSystem::SetShowShadowCaster(bool value) {
  show_shadow_caster_ = value;

  /* shadow_shader_.Bind(); */
  /* shadow_shader_.SetInt("u_show_shadow_caster", (int)show_shadow_caster_); */
  /* shadow_shader_.Unbind(); */


}
bool RendererSystem::OnWindowResize(int width, int height) {
  frame_buffer_.Resize(width, height);
  shadow_fb_.Resize(width, height);
  single_fb_[0].Resize(width, height);
  single_fb_[1].Resize(width, height);
  return false;
}

bool RendererSystem::OnKeyDown(int key, bool repeat) {
  if (!repeat && key == arc::Keys::T) {
    SetShowShadowCaster(!show_shadow_caster_);
  }

  return false;
}
