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

    main_fb_.Setup({wwidth,
                    wheight,
                    1,
                    {{arc::TexConf::RGBA, arc::TexConf::BYTE,
                      arc::TexConf::NEAREST}, // albedo
                     {arc::TexConf::RGBA, arc::TexConf::BYTE,
                      arc::TexConf::NEAREST}, // normal
                     {arc::TexConf::RGBA, arc::TexConf::FLOAT,
                      arc::TexConf::NEAREST}, // position
                     {arc::TexConf::RGBA, arc::TexConf::BYTE,
                      arc::TexConf::NEAREST}, // diffuse
                     {arc::TexConf::RGBA, arc::TexConf::BYTE,
                      arc::TexConf::NEAREST}, // ambient
                     {arc::TexConf::RGBA, arc::TexConf::BYTE,
                      arc::TexConf::NEAREST}}, // debug
                    true});

    for (int i = 0; i < 3; ++i)
      ambient_fb_[i].Setup({wwidth,
                                wheight,
                                1,
                                {{arc::TexConf::RGBA, arc::TexConf::BYTE,
                                  arc::TexConf::NEAREST}}, // shadow
                                true});

    for (int i = 0; i < 3; ++i)
      diffuse_fb_[i].Setup({wwidth,
                           wheight,
                           1,
                           {{arc::TexConf::RGBA, arc::TexConf::BYTE,
                             arc::TexConf::NEAREST}}, // shadow
                           true});

    for (int i = 0; i < 10; ++i)
      old_light_fbs_[i].Setup({wwidth,
                            wheight,
                            1,
                            {{arc::TexConf::RGBA, arc::TexConf::BYTE,
                              arc::TexConf::NEAREST}}, // shadow
                            true});

    combine_fb_.Setup({wwidth,
                       wheight,
                       1,
                       {{arc::TexConf::RGBA, arc::TexConf::BYTE,
                         arc::TexConf::NEAREST}}, // color
                       true});

  }

  tracing_shader_.Setup("/home/tilen/Projects/diploma/renderer/"
                        "engine/shaders/tracing_shader");
  ambient_filter_.Setup("/home/tilen/Projects/diploma/renderer/"
                            "engine/shaders/ambient_filter");
  diffuse_filter_.Setup("/home/tilen/Projects/diploma/renderer/"
                            "engine/shaders/diffuse_filter");
  combine_shader_.Setup("/home/tilen/Projects/diploma/renderer/"
                        "engine/shaders/combine_shader");
  fsaa_shader_.Setup("/home/tilen/Projects/diploma/renderer/"
                     "engine/shaders/fsaa_shader");
  copy_shader_.Setup("/home/tilen/Projects/diploma/renderer/"
                     "engine/shaders/copy_shader");
  temporal_shader_.Setup("/home/tilen/Projects/diploma/renderer/"
                         "engine/shaders/temporal_shader");
  combine_light_shader_.Setup("/home/tilen/Projects/diploma/renderer/"
                       "engine/shaders/combine_light_shader");

  overlay_renderer_.Init();
  blue_noise_tex_.Setup2D("LDR_RGB1_0.png", {});
  bayer_mat_tex_.Setup2D("bayer16tile2.png", {});
}
void RendererSystem::Dispose() { overlay_renderer_.Destroy(); }
void RendererSystem::Update(arc::Scene &scene) {

  frame_number_++;
  camera_.Update(arc::Engine::delta_time());
  if (camera_.CheckIfMoved()) {
    ss_target_old_ = ss_current_old_;
    logi("moved!");
  }
  {
    main_fb_.Bind();
    arc::RendererAPI::SetClearColor(glm::vec4(opt_.bacground_color,0));
    arc::RendererAPI::Clear();
    overlay_renderer_.Begin(tracing_shader_);
    {
      tracing_shader_.SetInt("u_frame_num", frame_number_);
      tracing_shader_.SetMat4("u_cam.view_projection",
                              camera_.camera().view_projection());
      tracing_shader_.SetFloat3("u_cam.origin",
                                glm::inverse(camera_.camera().view())[3]);
      glm::vec3 pos = glm::inverse(camera_.camera().view())[3];

      tracing_shader_.SetInt("u_world_tex", 1);
      tracing_shader_.SetInt("u_blue_noise", 2);

      tracing_shader_.SetInt("u_soft_shadows", opt_.use_soft_shadow);
      blue_noise_tex_.Bind(3);
      tracing_shader_.SetFloat2(
          "u_bn_size", {blue_noise_tex_.width(), blue_noise_tex_.height()});
      // settings
      tracing_shader_.SetInt("u_num_ao_rays", opt_.ambient_num_rays);
      tracing_shader_.SetFloat3("u_ambient_color", opt_.ambient_color);
      tracing_shader_.SetFloat("u_ao_ray_length", opt_.ambient_ray_distance);
      tracing_shader_.SetInt("u_use_temporal", (bool)opt_.temporal_num);
      tracing_shader_.SetInt("u_lum", (bool)opt_.ambient_based_on_color);
      // lights
      {

        auto lights =
            scene.registry()
                .view<const arc::TransformComponent, arc::LightComponent>();
        int i = 0;
        for (auto [entity, transform, light] : lights.each()) {
          std::string obj_str = "u_lights[" + std::to_string(i) + "].";
          tracing_shader_.SetFloat3(obj_str + "color", light.color);
          tracing_shader_.SetFloat(obj_str + "long_range", light.long_range);
          tracing_shader_.SetFloat(obj_str + "short_range", light.short_range);
          tracing_shader_.SetFloat(obj_str + "radius", light.radius);
          tracing_shader_.SetFloat3(obj_str + "position",
                                    transform.transform[3]);
          i++;
        }
        tracing_shader_.SetInt("u_num_lights", i);
      }
      // objects
      {

        auto objects =
            scene.registry()
                .view<const arc::TransformComponent, arc::ModelComponent>();
        int i = 0;
        for (auto [entity, transform, model] : objects.each()) {
          std::string obj_str = "u_objects[" + std::to_string(i) + "].";
          tracing_shader_.SetMat4(obj_str + "transform", transform.transform);
          tracing_shader_.SetFloat3(obj_str + "tex_size",
                                    model.raw_size);
          tracing_shader_.SetFloat3(obj_str + "vox_size", model.voxel_size);
          tracing_shader_.SetFloat3(obj_str + "inv_size",
                                    1.0f/model.size);
          tracing_shader_.SetInt(obj_str + "tex", i+3);

          arc::Texture::Bind(model.id,i+3);

          i++;
        }

        tracing_shader_.SetInt("u_num_objects", i);
      } 
      {

        auto materials =
            scene.registry()
                .view<const  arc::MaterialComponent, arc::TagComponent>();
        for (auto [entity, material, tag] : materials.each()) {
          tracing_shader_.SetFloat4(tag.tag+".color", material.mat.color);
        }

      
      }
    }
    overlay_renderer_.End(tracing_shader_);
    main_fb_.Unbind();
  }


  if (opt_.ambient_filter) // ambient denoise
  {
    for (int i = 0; i < opt_.ambient_num_passes; ++i) {
      int j = i + 1 == opt_.ambient_num_passes ? 2 : i & 1;
      ambient_fb_[j].Bind();
      arc::RendererAPI::SetClearColor(glm::vec4(opt_.bacground_color,0));
      arc::RendererAPI::Clear();
      overlay_renderer_.Begin(ambient_filter_);
      {
        ambient_filter_.SetFloat2(
            "u_window_size",
            {arc::Engine::window().width(), arc::Engine::window().height()});
        ambient_filter_.SetFloat("u_pos_th", opt_.ambient_filter_position_treshold);
        ambient_filter_.SetInt("u_texture", 1);
        if (i == 0)
          main_fb_.Bind(4, 1);
        else
          ambient_fb_[(i + 1) & 1].Bind(0, 1);
        ambient_filter_.SetInt("u_normal", 2);
        main_fb_.Bind(1, 2);
        ambient_filter_.SetInt("u_position", 3);
        main_fb_.Bind(2, 3);
        ambient_filter_.SetInt("u_filter_radius", opt_.ambient_filter_radius);
        ambient_filter_.SetInt("u_filter_cross", opt_.ambient_filter_type!=3?opt_.ambient_filter_type:(i&1)+1);
      }
      overlay_renderer_.End(ambient_filter_);
      ambient_fb_[j].Unbind();
    }
  }

  if(opt_.diffuse_filter){ // diffuse denoise
    for (int i = 0; i < opt_.diffuse_num_passes; ++i) {
      int j = i + 1 == opt_.diffuse_num_passes ? 2 : i & 1;
      diffuse_fb_[j].Bind();
      arc::RendererAPI::SetClearColor(glm::vec4(opt_.bacground_color,0));
      arc::RendererAPI::Clear();
      overlay_renderer_.Begin(diffuse_filter_);
      {
        diffuse_filter_.SetFloat2(
            "u_window_size",
            {arc::Engine::window().width(), arc::Engine::window().height()});
        diffuse_filter_.SetFloat("u_pos_th", opt_.diffuse_filter_position_treshold);
        diffuse_filter_.SetInt("u_texture", 1);
        if (i == 0)
          main_fb_.Bind(3, 1);
        else
          diffuse_fb_[(i + 1) & 1].Bind(0, 1);
        diffuse_filter_.SetInt("u_normal", 2);
        main_fb_.Bind(1, 2);
        diffuse_filter_.SetInt("u_position", 3);
        main_fb_.Bind(2, 3);
        diffuse_filter_.SetInt("u_filter_radius", opt_.diffuse_filter_radius);
        diffuse_filter_.SetInt("u_filter_cross", opt_.diffuse_filter_type!=3?opt_.diffuse_filter_type:(i&1)+1);
      }
      overlay_renderer_.End(diffuse_filter_);
      diffuse_fb_[j].Unbind();
    }
  }



  { //  combine and copy current light
    old_light_fbs_[ss_current_old_].Bind();
    arc::RendererAPI::Clear();
    overlay_renderer_.Begin(combine_light_shader_);
    {

      combine_light_shader_.SetInt("u_diffuse", 1);
      if (opt_.diffuse_filter && opt_.diffuse_num_passes)
        diffuse_fb_[2].Bind(0, 1);
      else
        main_fb_.Bind(3, 1);

      combine_light_shader_.SetInt("u_ambient", 2);
      if (opt_.ambient_filter && opt_.ambient_num_passes)
        ambient_fb_[2].Bind(0, 2);
      else
        main_fb_.Bind(4, 2);

      combine_light_shader_.SetFloat("u_ambient_scalar", opt_.ambient_scalar);
      combine_light_shader_.SetFloat("u_diffuse_scalar", opt_.diffuse_scalar);
      combine_light_shader_.SetInt("u_diffuse", 1);
    }
    overlay_renderer_.End(combine_light_shader_);
    old_light_fbs_[ss_current_old_].Unbind();
    ss_current_old_ = (ss_current_old_ + 1) % 10;
    if (ss_target_old_ != -1 && ss_current_old_ == ss_target_old_) {
      ss_target_old_ = -1;
    }
  }

  { //  combine old light
    diffuse_fb_[2].Bind();
    arc::RendererAPI::Clear();
    overlay_renderer_.Begin(temporal_shader_);
    {
      int num = ss_target_old_ == -1
                    ? opt_.temporal_num
                    : glm::min((ss_current_old_ - ss_target_old_ + 10) % 10,opt_.temporal_num);
      int curr = (ss_current_old_ - 1 + 10) % 10;
      temporal_shader_.SetInt("u_num", num);
      logi("num: {0}", num);
      for (int i = 0; i < num; ++i) {
        std::string str = "u_old[" + std::to_string(i) + "]";
        temporal_shader_.SetInt(str, i + 1);
        old_light_fbs_[curr].Bind(0, i + 1);
        curr = (curr - 1 + 10) % 10;
      }
    }
    overlay_renderer_.End(temporal_shader_);
    diffuse_fb_[2].Unbind();
  }



  {
    if (opt_.use_fsaa)
      combine_fb_.Bind();

    arc::RendererAPI::SetClearColor(glm::vec4(opt_.bacground_color,0));
    arc::RendererAPI::Clear();
    overlay_renderer_.Begin(combine_shader_);
    {
      combine_shader_.SetInt("u_albedo", 1);
      main_fb_.Bind(0, 1);
      combine_shader_.SetInt("u_light", 2);
      diffuse_fb_[2].Bind(0, 2);
      combine_shader_.SetInt("u_debug", 5);
      main_fb_.Bind(5, 5);
      combine_shader_.SetInt("u_dither", 6);
      bayer_mat_tex_.Bind(6);
      combine_shader_.SetInt("u_dithering", opt_.dithering);
    }
    overlay_renderer_.End(combine_shader_);
    if (opt_.use_fsaa)
      combine_fb_.Unbind();
  }

  if (opt_.use_fsaa) { // FSAA
    arc::RendererAPI::SetClearColor(glm::vec4(opt_.bacground_color,0));
    arc::RendererAPI::Clear();
    overlay_renderer_.Begin(fsaa_shader_);
    {
      fsaa_shader_.SetInt("u_input", 1);
      combine_fb_.Bind(0, 1);
      fsaa_shader_.SetFloat2("u_resolution", {arc::Engine::window().width(),
                                              arc::Engine::window().height()});
    }
    overlay_renderer_.End(fsaa_shader_);
  }
}

bool RendererSystem::OnWindowResize(int width, int height) {
  main_fb_.Resize(width, height);

  combine_fb_.Resize(width, height);
  for (int i = 0; i < 3; ++i)
    ambient_fb_[i].Resize(width, height);
  for (int i = 0; i < 3; ++i)
    diffuse_fb_[i].Resize(width, height);
  for (int i = 0; i < 10; ++i)
    old_light_fbs_[i].Resize(width, height);

  return false;
}

bool RendererSystem::OnKeyDown(int key, bool repeat) { return false; }
