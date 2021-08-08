
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
  void SetShowShadowCaster(bool value); 
  arc::FrameBuffer frame_buffer_, shadow_fb_, single_fb_[2];
  arc::VoxelRenderer renderer_;
  arc::OverlayRenderer overlay_renderer_;
  arc::Shader shadow_shader_, voxel_shader_, gauss_shader_, combine_shader_, blurr_shader_;
  arc::ShadowTexture shadow_texture_;
  arc::Texture blue_noise_tex_;
  CameraController camera_;
  bool setup_{false};
  bool show_shadow_caster_{false};




  friend class Game;
};
#endif
