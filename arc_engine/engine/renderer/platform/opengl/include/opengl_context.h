//
// Created by adolin on 22. 12. 20.
//

#ifndef ARC_ENGINE_OPENGL_CONTEXT_H
#define ARC_ENGINE_OPENGL_CONTEXT_H

#include "graphics_context.h"
#include <glfw3.h>
namespace arc{
    class OpenGLContext: public GraphicsContext{
    public:
        OpenGLContext(GLFWwindow* window_handle);

        void Init() override;

        void SwapBuffers() override;


    private:
        GLFWwindow* window_handle_;
    };
}


#endif //ARC_ENGINE_OPENGL_CONTEXT_H
