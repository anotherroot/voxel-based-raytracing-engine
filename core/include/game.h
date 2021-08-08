//
// Created by adolin on 21. 12. 20.
//

#ifndef ARC_ENGINE_BATTLEMAPAPP_H
#define ARC_ENGINE_BATTLEMAPAPP_H

#include "application.h"
#include "renderer_system.h"
#include "entity.h"
class Game : public arc::Application{
public:
  Game() = default;
  void Init() override;
  void Dispose() override;
  void Update() override;
  void ImGuiRender() override;

private:
  arc::Scene scene_;
  arc::VoxelModel guy_model_, floor_;
  RendererSystem renderer_system_;
  arc::Entity light_;
  std::vector<arc::Entity> walters_;

  float num_frames_{0};
  float fps_{0};
  float second_{0};
};

#endif // ARC_ENGINE_BATTLEMAPAPP_H
