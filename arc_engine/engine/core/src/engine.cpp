//
// Created by adolin on 21. 12. 20.
//

#include "engine.h"
#include "../../renderer/include/renderer_api.h"

#include "event_handler.h"

namespace arc{
    Engine* Engine::instance_{nullptr};


    void Engine::Run() {
        running_ = true;

        imgui_layer_.Init();
        app_caller_.Init();
        while (running_){

            float time = window_.GetTime();
            delta_time_ = time - last_frame_time_;
            last_frame_time_ = time;

            app_caller_.Update();


            imgui_layer_.Begin();
            {
                app_caller_.ImGuiRender();
            }
            imgui_layer_.End();


            window_.OnUpdate();
        }

        app_caller_.Dispose();
        imgui_layer_.Dispose();

    }

    bool Engine::OnWindowResize(int width, int height) {
        window_.Resize(width,height);
        RenderCommand::SetViewport(0,0,width,height);
        return false;
    }

    bool Engine::OnWindowClose() {
        running_ = false;
        return true;
    }

    void Engine::SetAppConfig(const Engine::AppConfig &appConfig) {
        config_ = appConfig;
        if(config_.title.empty()){
            config_.title = "Untitled project";
        }
    }




}
