//
// Created by adolin on 21. 12. 20.
//

#ifndef ARC_ENGINE_OPENGL3_RENDERER_API_H
#define ARC_ENGINE_OPENGL3_RENDERER_API_H

#include "../../renderer/include/renderer_api.h"

namespace arc{
    class OpenGL3RendererAPI : public RendererAPI{
    public:

        void Init() override;
        void SetViewport(uint x, uint y, uint width, uint height) override;
        void SetClearColor(const glm::vec4& color) override;
        void Clear() override;

        void DrawIndexed(const VertexArray&, uint count) override;

    };
}



#endif //ARC_ENGINE_OPENGL3_RENDERER_API_H
