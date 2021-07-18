//
// Created by adolin on 21. 12. 20.
//

#ifndef ARC_ENGINE_BATTLEMAPAPP_H
#define ARC_ENGINE_BATTLEMAPAPP_H

#include "camera_controller.h"
#include "frame_buffer.h"
#include "application.h"
#include "voxel_renderer.h"
#include "voxel_model.h"
#include "shadow_texture.h"

class Game : public arc::Application, arc::WindowListener {
public:
  Game();
  void Init() override;
  void Dispose() override;
  void Update() override;
  void ImGuiRender() override;
  bool OnWindowResize(int width, int height)override;

private:
  arc::FrameBuffer frame_buffer_;
  arc::VoxelRenderer renderer_;
  arc::Shader shadow_shader_, voxel_shader_;
  arc::ShadowTexture shadow_texture_;
  CameraController camera_;
  arc::VoxelModel model_, waterfall_;
  std::vector<arc::Texture> textures_;
  std::vector<arc::VoxelTransform> transforms_;
  std::vector<arc::Material> materials_;
};

#endif // ARC_ENGINE_BATTLEMAPAPP_H
