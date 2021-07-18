//
// Created by adolin on 22. 12. 20.
//

#include "graphics_context.h"

#include "renderer_api.h"


#ifdef ARC_PLATFORM_ANDROID
    #ifdef ARC_PLATFORM_SDL
        #ifdef ARC_PLATFORM_OPENGL_ES
            #include "opengl_es_context.h"
        #endif
    #endif
#elif ARC_PLATFORM_LINUX
    #ifdef ARC_PLATFORM_SDL
        #ifdef ARC_PLATFORM_OPENGL
            #include "opengl_context_sdl.h"
        #endif
    #elif ARC_PLATFORM_GLFW
        #ifdef ARC_PLATFORM_OPENGL
            #include "opengl_context.h"
        #endif
    #endif
#elif ARC_PLATFORM_WINDOWS
#endif



arc::GraphicsContext *arc::GraphicsContext::CreateOpenGLContext(void *window) {
#ifdef ARC_PLATFORM_SDL
    #ifdef ARC_PLATFORM_OPENGL_ES
        return new OpenGLESContext((SDL_Window*)window);
    #elif ARC_PLATFORM_OPENGL
        return new OpenGLSDLContext((SDL_Window*)window);
    #else
        arc_core_assert(false, "Not supported");
        return nullptr;
    #endif
#elif ARC_PLATFORM_GLFW
    #ifdef ARC_PLATFORM_OPENGL
    return new OpenGLContext((GLFWwindow*)window);
    #else
        arc_core_assert(false, "Not supported");
        return nullptr;
    #endif
#else
        arc_core_assert(false, "RendererAPI not supported");
        return nullptr;
#endif
}

