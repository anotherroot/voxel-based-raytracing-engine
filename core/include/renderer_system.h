
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
  arc::Shader tracing_shader_, fsaa_shader_, combine_shader_, soft_shadow_shader_, copy_shader_, temporal_shader_, median_shader_,new_median_shader_;
  arc::Texture blue_noise_tex_, bayer_mat_tex_;
  arc::FrameBuffer main_fb_, combine_fb_, soft_shadow_fb_[3], ss_old_fbs_[10], median_fb_[3];
  CameraController camera_;

  int ss_current_old_{0};
  int ss_target_old_{0};

  struct Options{
    // ambinet oclusion
    int AO_num_rays{2};
    float AO_ray_dist{0.3};
    // ambient light
    glm::vec3 ambient_color{1,1,1};

    int change_ambient_based_on_color{1};
    float diffuse_scalar{0.5};
    float ambient_scalar{0.5};
    float specular_scalar{0.5};

    //shadow
    bool use_soft_shadow{true};
    int ss_num_passes{2};
    int ss_filter_radius{15};
    int ss_cross_type{3};
    int ss_num_old{10};
    float ss_pos_th{0.08};

    int median_num_passes{0};
    int median_radius{2};
    int new_median_shader{0};
    int dithering{1}; 

    bool use_fsaa{true};
  };
  Options opt_;



  int frame_number_{0};
  bool setup_{false};
  friend class Game; 
  friend class CameraController;
};
#endif
