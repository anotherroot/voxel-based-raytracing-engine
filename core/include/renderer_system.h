
#ifndef RENDERER_SYSTEM_H
#define RENDERER_SYSTEM_H
#include "camera_controller.h"
#include "frame_buffer.h"
#include "listeners.h"
#include "scene.h"
#include "shader.h"
#include "voxel_renderer.h"
#include "voxel_model.h"
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
  arc::Shader tracing_shader_, fsaa_shader_, combine_shader_, bilateral_filter_, ambient_filter_, diffuse_filter_, copy_shader_, temporal_shader_, combine_light_shader_;
  arc::Texture blue_noise_tex_, bayer_mat_tex_;
  arc::FrameBuffer main_fb_, combine_fb_, bilateral_fb_[3], old_fbs_[2];
  CameraController camera_;

  int num_old_{0};

  struct Options{
    glm::vec3 bacground_color{0,0,0};
    int traversal_algo{0};
    int reflection_depth_{2};
    // diffuse
    int use_soft_shadow{1};// soft to hard shadows
    int diffuse_filter{0}; // uses filter on diffuse
    int diffuse_num_passes{2};
    float diffuse_filter_position_treshold{0.08};
    int diffuse_filter_radius{2};
    int diffuse_filter_type{1};
    float diffuse_scalar{0.5};

    // ambient
    int ambient_filter{0}; // uses filter on ambient light
    int ambient_num_passes{2};
    float ambient_filter_position_treshold{0.08};
    int ambient_filter_radius{15};
    int ambient_filter_type{3};
    float ambient_ray_distance{10.0};
    int ambient_num_rays{1};
    glm::vec3 ambient_color{1,1,1};
    float ambient_scalar{0.5};
    float ambient_start{0};

    //Bilateral filter
    int num_bilateral_passes{0};
    float bilateral_treshold{0.1};


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
