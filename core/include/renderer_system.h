
#ifndef RENDERER_SYSTEM_H
#define RENDERER_SYSTEM_H
#include "camera_controller.h"
#include "frame_buffer.h"
#include "listeners.h"
#include "scene.h"
#include "shader.h"
#include "shadow_texture.h"
#include "voxel_renderer.h"
#include "overlay_renderer.h"

#define MAX_NUM_AOS 100

class RendererSystem : public arc::WindowListener, arc::KeyboardListener {
public:
  void Setup(arc::Scene &scene);
  void Dispose();
  ~RendererSystem() { Dispose(); }
  operator bool() const { return setup_; }
  void Update(arc::Scene &scene);
  bool OnWindowResize(int width, int height) override;
  bool OnKeyDown(int key, bool repeat) override;

private:

  arc::OverlayRenderer overlay_renderer_;
  arc::Shader tracing_shader_, fsaa_shader_, combine_shader_, ambient_filter_, diffuse_filter_, copy_shader_, temporal_shader_, combine_light_shader_;
  arc::Texture blue_noise_tex_, bayer_mat_tex_;
  arc::FrameBuffer main_fb_, combine_fb_, ambient_fb_[3], old_light_fbs_[10], diffuse_fb_[3];
  CameraController camera_;

  int ss_current_old_{0};
  int ss_target_old_{0};

  struct Options{
    glm::vec3 bacground_color{0,0,0};
    // diffuse
    int use_soft_shadow{1};// soft to hard shadows
    int diffuse_filter{0}; // uses filter on diffuse
    int diffuse_num_passes{2};
    float diffuse_filter_position_treshold{0.08};
    int diffuse_filter_radius{2};
    int diffuse_filter_type{1};
    float diffuse_scalar{0.5};

    // ambient
    int ambient_filter{1}; // uses filter on ambient light
    int ambient_num_passes{2};
    float ambient_filter_position_treshold{0.08};
    int ambient_filter_radius{8};
    int ambient_filter_type{3};
    float ambient_ray_distance{0.3};
    int ambient_num_rays{2};
    glm::vec3 ambient_color{1,1,1};
    int ambient_based_on_color{1};
    float ambient_scalar{0.5};

    //temporal
    int temporal_num{10};

    //postprocessing
    int dithering{1}; 
    int use_fsaa{true};

  };
  Options opt_;



  int frame_number_{0};
  bool setup_{false};
  friend class Game; 
  friend class CameraController;
};
#endif
