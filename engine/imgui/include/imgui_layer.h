//
// Created by adolin on 20. 01. 21.
//

#ifndef ARC_ENGINE_IMGUI_LAYER_H
#define ARC_ENGINE_IMGUI_LAYER_H

namespace arc{
    class ImGuiLayer {
    public:

        void Init();
        void Dispose();

        void Begin();
        void End();

        void BlockEvents(bool block) { m_BlockEvents = block; }

        void SetDarkThemeColors();
    private:
        bool m_BlockEvents = true;
        float m_Time = 0.0f;
    };

}




#endif //ARC_ENGINE_IMGUI_LAYER_H
