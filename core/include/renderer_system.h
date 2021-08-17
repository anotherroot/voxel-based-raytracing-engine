
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
  int frame_number_{0};
  int curr_AO_fb_{0};
  int target_AO_fb_{0};
  bool ignore_old_AO_{true};
  void SetShowShadowCaster(bool value); 
  arc::FrameBuffer frame_buffer_, shadow_fb_, single_fb_[2], AO_fbs_[10];
  arc::VoxelRenderer renderer_;
  arc::OverlayRenderer overlay_renderer_;
  arc::Shader shadow_shader_, voxel_shader_, gauss_shader_, combine_shader_, blurr_shader_, copy_shader_, combine_AO_shader_, bilateral_shader_, fsaa_shader_;
  arc::ShadowTexture shadow_texture_;
  arc::Texture blue_noise_tex_;
  CameraController camera_;
  bool setup_{false};
  bool show_shadow_caster_{false};


  struct Options{
    // ambinet oclusion
    int AO_num_rays{3};
    float AO_bilateral_tolerance{0.1};
    bool AO_with_bilateral_{true};
    float AO_ray_dist{0.5};
    int max_temporal{10};
    // ambient light
    glm::vec3 ambient_color{1,1,1};

    float diffuse_scalar{0.5};
    float ambient_scalar{0.5};
    float specular_scalar{0.5};
  
  };
  Options opt_;



  friend class Game; 
  friend class CameraController;
};
#endif
