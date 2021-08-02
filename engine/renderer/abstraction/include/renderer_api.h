//
// Created by adolin on 21. 12. 20.
//

#ifndef ARC_ENGINE_OPENGL_RENDERER_API_H
#define ARC_ENGINE_OPENGL_RENDERER_API_H

#include "structs.h"
#include "glm.hpp"
#include "vertex_array.h"
namespace arc{
    class RendererAPI{
    public:
        static void Setup();
        static void Dispose();
        static void SetViewport(uint x, uint y, uint width, uint height) ;
        static void SetClearColor(const glm::vec4& color) ;
        static void Clear() ;

        static void DrawIndexed(const VertexArray&, uint count) ;
    private:
        static bool setup_;

    };
}



#endif //ARC_ENGINE_OPENGL_RENDERER_API_H
