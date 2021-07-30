
#ifndef RENDERER_SYSTEM_H
#define RENDERER_SYSTEM_H
#include "scene.h"
#include "shader.h"
#include "voxel_renderer.h"
#include "frame_buffer.h"
#include "shadow_texture.h"
#include "camera_controller.h"
#include "listeners.h"
class RendererSystem : public arc::WindowListener{
public:
  void Setup();
  void Dispose();
  ~RendererSystem() { Dispose(); }
  operator bool() const { return setup_; }
  void Update(arc::Scene &scene);
  bool OnWindowResize(int width, int height); 

private:
  arc::FrameBuffer frame_buffer_;
  arc::VoxelRenderer renderer_;
  arc::Shader shadow_shader_, voxel_shader_;
  arc::ShadowTexture shadow_texture_;
  CameraController camera_;
  bool setup_{false};
};
#endif
