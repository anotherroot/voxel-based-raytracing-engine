//
// Created by adolin on 21. 12. 20.
//

#ifndef ARC_ENGINE_RENDERER_API_H
#define ARC_ENGINE_RENDERER_API_H
#include <glm.hpp>
#include "core.h"
#include "vertex_array.h"

namespace arc{
    class RendererAPI {
    public:
        enum class API {
            None, OpenGL, Vulkan, OpenGL_ES, WebGL
        };

        virtual void Init() = 0;
        virtual void SetViewport(uint x, uint y, uint width, uint height) = 0;
        virtual void SetClearColor(const glm::vec4& color) = 0;
        virtual void Clear() = 0;
        virtual void DrawIndexed(const VertexArray&, uint count) = 0;




        inline static API api() {return s_api_;}
        inline static void set_api(API api) {s_api_ = api;}


        static RendererAPI* Create();
    private:
        static API s_api_;
    };



    class RenderCommand : public Caller{
        typedef void(VoidMemberFn)(void*);
        typedef void(VoidMemberFnVec4)(void*, const glm::vec4&color);
        typedef void(VoidMemberFnViewport)(void*, uint, uint, uint, uint);
        typedef void(VoidMemberVAUnIntFn)(void*, const VertexArray&, uint);

        static RendererAPI* s_renderer_api_;

        static VoidMemberFn* s_init_fn_;
        static VoidMemberFnViewport * s_set_viewport_fn_;
        static VoidMemberFnVec4 * s_set_clear_color_fn_;
        static VoidMemberFn* s_clear_fn_;
        static VoidMemberVAUnIntFn* s_draw_indexed_fn_;

    public:


        inline static void SetViewport(uint x, uint y, uint width, uint height){
            s_set_viewport_fn_(s_renderer_api_, x, y, width, height);
        }

        inline static void SetClearColor(const glm::vec4& color) {
            s_set_clear_color_fn_(s_renderer_api_, color);
        }
        inline static void Clear(){
            s_clear_fn_(s_renderer_api_);
        }

        inline static void DrawIndexed(const VertexArray& vertex_array, uint count){
            s_draw_indexed_fn_(s_renderer_api_, vertex_array, count);
        }


    private:

        static void Init();



        friend class Engine;

    };
}



#endif //ARC_ENGINE_RENDERER_API_H
