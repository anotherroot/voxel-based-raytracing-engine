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
  arc::VoxelModel guy_model_;
  RendererSystem renderer_system_;
  arc::Entity guy_;
};

#endif // ARC_ENGINE_BATTLEMAPAPP_H
