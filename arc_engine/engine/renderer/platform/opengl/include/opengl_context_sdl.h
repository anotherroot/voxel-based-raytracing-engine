//
// Created by adolin on 22. 12. 20.
//

#ifndef ARC_ENGINE_OPENGL_CONTEXT_SDL_H
#define ARC_ENGINE_OPENGL_CONTEXT_SDL_H

#include "graphics_context.h"
#include <SDL.h>

namespace arc{
    class OpenGLSDLContext: public GraphicsContext{
    public:
        OpenGLSDLContext(SDL_Window * window_handle);

        void Init() override;

        void SwapBuffers() override;

    private:
        SDL_Window* window_handle_;
    };
}


#endif //ARC_ENGINE_OPENGL_CONTEXT_SDL_H
