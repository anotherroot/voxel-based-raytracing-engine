#include "renderer_system.h"
#include "components.h"
#include "engine.h"
#include "entity.h"
#include "glad/glad.h" // TODO: remove
#include "keys.h"
#include "voxel_model.h"
#define SMALL 1
void RendererSystem::Setup(arc::Scene &scene) {

  arc::EventHandler::SubscribeWindow(this);
  arc::EventHandler::SubscribeKeyboard(this);

  {
    uint wwidth = arc::Engine::window().width();
    uint wheight = arc::Engine::window().height();

    main_fb_.Setup({wwidth/SMALL,
                    wheight/SMALL,
                    1,
                    {{arc::TexConf::RGBA, arc::TexConf::BYTE,
                      arc::TexConf::NEAREST}, // albedo
                     {arc::TexConf::RGBA, arc::TexConf::BYTE,
                      arc::TexConf::NEAREST}, // normal
                     {arc::TexConf::RGBA, arc::TexConf::FLOAT,
                      arc::TexConf::NEAREST}, // position
                     {arc::TexConf::RGBA, arc::TexConf::FLOAT,
                      arc::TexConf::NEAREST}, // diffuse
                     {arc::TexConf::RGBA, arc::TexConf::FLOAT,
                      arc::TexConf::NEAREST}, // ambient
                     {arc::TexConf::RGBA, arc::TexConf::FLOAT,
                      arc::TexConf::NEAREST}, // reflection
                     {arc::TexConf::RGBA, arc::TexConf::BYTE,
                      arc::TexConf::NEAREST}}, // comp
                    true});

    for (int i = 0; i < 3; ++i)
      bilateral_fb_[i].Setup(
          {wwidth/SMALL,
           wheight/SMALL,
           1,
           {
               {arc::TexConf::RGBA, arc::TexConf::BYTE, arc::TexConf::NEAREST},
           }, // shadow
           false});

    for (int i = 0; i < 2; ++i)
      old_fbs_[i].Setup(
          {wwidth/SMALL,
           wheight/SMALL,
           1,
           {{arc::TexConf::RGBA, arc::TexConf::FLOAT, arc::TexConf::NEAREST},
            {arc::TexConf::RGBA, arc::TexConf::FLOAT, arc::TexConf::NEAREST},
            {arc::TexConf::RGBA, arc::TexConf::FLOAT,
             arc::TexConf::NEAREST}}, // shadow
           true});

    combine_fb_.Setup({wwidth/SMALL,
                       wheight/SMALL,
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
  bilateral_filter_.Setup("/home/tilen/Projects/diploma/renderer/"
                          "engine/shaders/bilateral_shader");

  overlay_renderer_.Init();
  blue_noise_tex_.Setup2D("LDR_RGB1_0.png", {});
  bayer_mat_tex_.Setup2D("bayer16tile2.png", {});
}
void RendererSystem::Dispose() { overlay_renderer_.Destroy(); }
void RendererSystem::Update(arc::Scene &scene) {

  frame_number_++;
  num_old_++;
  camera_.Update(arc::Engine::delta_time());
  if (camera_.CheckIfMoved()) {
    num_old_ = 1;
    logi("moved!");
  }
  {
    main_fb_.Bind();
    arc::RendererAPI::SetClearColor(glm::vec4(opt_.bacground_color, 0));
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
      blue_noise_tex_.Bind(2);
      tracing_shader_.SetFloat2(
          "u_bn_size", {blue_noise_tex_.width(), blue_noise_tex_.height()});
      // settings
      tracing_shader_.SetInt("u_num_ao_rays", opt_.ambient_num_rays);
      tracing_shader_.SetInt("u_reflection_depth", opt_.reflection_depth_);
      tracing_shader_.SetFloat3("u_ambient_color", opt_.ambient_color);
      tracing_shader_.SetFloat("u_ambient_start", opt_.ambient_start);
      tracing_shader_.SetFloat("u_ao_ray_length", opt_.ambient_ray_distance);
      tracing_shader_.SetFloat("u_ambient_scalar", opt_.ambient_scalar);
      tracing_shader_.SetFloat("u_diffuse_scalar", opt_.diffuse_scalar);
      tracing_shader_.SetFloat3("u_background_color", opt_.bacground_color);
      tracing_shader_.SetInt("u_traversal_algo", opt_.traversal_algo);
      // lights
      {

        auto lights =
            scene.registry()
                .view<const arc::TransformComponent, arc::LightComponent>();
        int i = 0;
        for (auto entt : lights) {
          arc::Entity entity(entt, &scene);
          auto &light = entity.Get<arc::LightComponent>();
          auto transform = entity.Get<arc::TransformComponent>();
          std::string obj_str = "u_lights[" + std::to_string(i) + "].";
          tracing_shader_.SetFloat3(obj_str + "color", light.color);
          tracing_shader_.SetFloat(obj_str + "range", light.range);
          tracing_shader_.SetFloat(obj_str + "radius", light.radius);
          tracing_shader_.SetFloat3(obj_str + "position",
                                    transform.transform[3]);

          if (light.moded) {
            num_old_ = 1;
            light.moded = false;
          }

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

        std::map<int, int> render_ids;
        for (auto [entity, transform, model] : objects.each()) {
          std::string obj_str = "u_objects[" + std::to_string(i) + "].";
          tracing_shader_.SetMat4(obj_str + "transform", transform.transform);
          tracing_shader_.SetFloat3(obj_str + "tex_size", model.raw_size);
          tracing_shader_.SetFloat3(obj_str + "vox_size", model.voxel_size);
          tracing_shader_.SetFloat3(obj_str + "inv_size", 1.0f / model.size);

          auto fi = render_ids.find(model.id);
          if (fi == render_ids.end()) {
            render_ids.insert({model.id, render_ids.size() + 3});
            fi = render_ids.find(model.id);
            
            tracing_shader_.SetInt("u_textures[" + std::to_string(fi->second-3) + "]",
                                   fi->second);
            arc::Texture::Bind(model.id, fi->second);
          }

          tracing_shader_.SetInt(obj_str + "tex", fi->second-3);

          if (model.changed) {
            num_old_ = 1;
            model.changed = false;
          }

          i++;
        }

        tracing_shader_.SetInt("u_num_objects", i);
      }
      {

        auto materials =
            scene.registry()
                .view<arc::MaterialComponent, arc::TagComponent>();
        for (auto [entity, material, tag] : materials.each()) {
          tracing_shader_.SetFloat3(tag.tag + ".color", material.mat.color);
          tracing_shader_.SetFloat(tag.tag + ".reflection",
                                   material.mat.reflection);
          tracing_shader_.SetFloat(tag.tag + ".refraction",
                                   material.mat.refraction);
          tracing_shader_.SetFloat(tag.tag + ".roughness",
                                   material.mat.roughness);
          if (material.changed) {
            num_old_ = 1;
            material.changed = false;
          }
        }
      }
    }
    overlay_renderer_.End(tracing_shader_);
    main_fb_.Unbind();
  }

  { //  combine current with old light data
    old_fbs_[num_old_ % 2].Bind();
    arc::RendererAPI::Clear();
    overlay_renderer_.Begin(combine_light_shader_);
    {

      combine_light_shader_.SetInt("u_diffuse", 1);
      main_fb_.Bind(3, 1);
      combine_light_shader_.SetInt("u_ambient", 2);
      main_fb_.Bind(4, 2);
      combine_light_shader_.SetInt("u_reflection", 3);
      main_fb_.Bind(5, 3);

      combine_light_shader_.SetInt("u_old_diffuse", 4);
      old_fbs_[(num_old_ + 1) % 2].Bind(0, 4);
      combine_light_shader_.SetInt("u_old_ambient", 5);
      old_fbs_[(num_old_ + 1) % 2].Bind(1, 5);
      combine_light_shader_.SetInt("u_old_reflection", 6);
      old_fbs_[(num_old_ + 1) % 2].Bind(2, 6);

      int num = glm::min(num_old_,
                         10000); // clamp num of rays to keep float precision
      combine_light_shader_.SetFloat("u_inv_num", 1.0f / num);
      combine_light_shader_.SetFloat("u_inv_old", num - 1);
    }
    overlay_renderer_.End(combine_light_shader_);
    old_fbs_[num_old_ % 2].Unbind();
  }

  {
    /* for (int i = 0; i < opt_.num_bilateral_passes; ++i) { */
    /*   int j = i + 1 == opt_.num_bilateral_passes ? 2 : i & 1; */

    /*   bilateral_fb_[j].Bind(); */

    /*   arc::RendererAPI::Clear(); */
    /*   overlay_renderer_.Begin(bilateral_filter_); */
    /*   { */
    /*     bilateral_filter_.SetFloat2("u_window_size",
     * {arc::Engine::window().width(), */
    /*                                           arc::Engine::window().height()});
     */
    /*     bilateral_filter_.SetFloat("u_q", opt_.bilateral_treshold); */
    /*     bilateral_filter_.SetInt("u_texture",1); */
    /*     if(i) */
    /*       bilateral_fb_[(i+1)&1].Bind(0,1); */
    /*     else */
    /*       old_fbs_[num_old_%2].Bind(0,1); */
    /*   } */
    /*   overlay_renderer_.End(bilateral_filter_); */
    /*   bilateral_fb_[j].Unbind(); */
    /* } */
  }

  {
    if (opt_.use_fsaa)
      combine_fb_.Bind();

    arc::RendererAPI::SetClearColor(glm::vec4(opt_.bacground_color, 0));
    arc::RendererAPI::Clear();
    overlay_renderer_.Begin(combine_shader_);
    {
      combine_shader_.SetInt("u_albedo", 1);
      main_fb_.Bind(0, 1);
      combine_shader_.SetInt("u_diffuse", 4);
      old_fbs_[num_old_ % 2].Bind(0, 4);
      combine_shader_.SetInt("u_ambient", 5);
      old_fbs_[num_old_ % 2].Bind(1, 5);
      combine_shader_.SetInt("u_reflection", 6);
      old_fbs_[num_old_ % 2].Bind(2, 6);
      combine_shader_.SetInt("u_dither", 7);
      bayer_mat_tex_.Bind(7);

      combine_shader_.SetInt("u_dithering", opt_.dithering);
      combine_shader_.SetFloat("u_ambient_scalar", opt_.ambient_scalar);
      combine_shader_.SetFloat("u_diffuse_scalar", opt_.diffuse_scalar);
      combine_shader_.SetFloat3("u_ambient_color", opt_.ambient_color);
      combine_shader_.SetFloat("u_ambient_start", opt_.ambient_start);
    }
    overlay_renderer_.End(combine_shader_);
    if (opt_.use_fsaa)
      combine_fb_.Unbind();
  }

  if (opt_.use_fsaa) { // FSAA
    arc::RendererAPI::SetClearColor(glm::vec4(opt_.bacground_color, 0));
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
  main_fb_.Resize(width/SMALL, height/SMALL);

  combine_fb_.Resize(width/SMALL, height);
  for (int i = 0; i < 3; ++i)
    bilateral_fb_[i].Resize(width/SMALL, height/SMALL);
  for (int i = 0; i < 2; ++i)
    old_fbs_[i].Resize(width/SMALL, height/SMALL);

  return false;
}

bool RendererSystem::OnKeyDown(int key, bool repeat) {
  if (key == arc::Keys::R) {
    num_old_ = 1;
  }

  return false;
}
