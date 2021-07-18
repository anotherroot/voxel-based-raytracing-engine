//
// Created by adolin on 22. 12. 20.
//

#ifndef ARC_ENGINE_GRAPHICS_CONTEXT_H
#define ARC_ENGINE_GRAPHICS_CONTEXT_H

namespace arc{
    class GraphicsContext{
    public:
        static GraphicsContext* CreateOpenGLContext(void* window);
        virtual void Init() =0;
        virtual void SwapBuffers() =0;

    };
}


#endif //ARC_ENGINE_GRAPHICS_CONTEXT_H
