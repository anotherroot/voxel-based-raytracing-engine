//
// Created by adolin on 21. 12. 20.
//

#include "renderer_api.h"
#ifdef ARC_PLATFORM_OPENGL
#include "opengl_renderer_api.h"
#elif ARC_PLATFORM_OPENGL_ES
#include "opengl_es_renderer_api.h"
#endif
#include "core.h"

namespace arc{
    RendererAPI::API RendererAPI::s_api_ = RendererAPI::API::OpenGL_ES;

    RendererAPI *RendererAPI::Create() {
#ifdef ARC_PLATFORM_OPENGL
        return new OpenGLRendererAPI();
#elif ARC_PLATFORM_OPENGL_ES
         return new OpenGLESRendererAPI();
#else
        arc_core_assert(false, "Not Supported");
        return nullptr;
#endif
    }


    RendererAPI* RenderCommand::s_renderer_api_ = nullptr;
    RenderCommand::VoidMemberFn* RenderCommand::s_init_fn_;
    RenderCommand::VoidMemberFnViewport * RenderCommand::s_set_viewport_fn_;
    RenderCommand::VoidMemberFnVec4 * RenderCommand::s_set_clear_color_fn_;
    RenderCommand::VoidMemberFn* RenderCommand::s_clear_fn_;
    RenderCommand::VoidMemberVAUnIntFn* RenderCommand::s_draw_indexed_fn_;
    void RenderCommand::Init() {
        s_renderer_api_ = RendererAPI::Create();

        s_init_fn_ = (VoidMemberFn*)GetVTable<RendererAPI>(s_renderer_api_)[0];
        s_set_viewport_fn_ = (VoidMemberFnViewport*)GetVTable<RendererAPI>(s_renderer_api_)[1];
        s_set_clear_color_fn_ = (VoidMemberFnVec4*)GetVTable<RendererAPI>(s_renderer_api_)[2];
        s_clear_fn_ = (VoidMemberFn*)GetVTable<RendererAPI>(s_renderer_api_)[3];
        s_draw_indexed_fn_ = (VoidMemberVAUnIntFn*)GetVTable<RendererAPI>(s_renderer_api_)[4];

        s_init_fn_(s_renderer_api_);
    }
}