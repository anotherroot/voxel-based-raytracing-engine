//
// Created by adolin on 3. 01. 21.
//

#ifndef ARC_ENGINE_FONT_RENDERER_H
#define ARC_ENGINE_FONT_RENDERER_H
#include <glm/glm.hpp>
#include "orthographic_camera.h"
#include "vertex_array.h"
#include "shader.h"
#include "texture.h"
namespace arc{
    struct VertexFont{
        glm::vec3 pos;
        glm::vec2 tex_coord;
    };
    struct TextureCont{
        typedef void(VoidMemberUintFn)(void*,uint);
        TextureClass* object;
        VoidMemberUintFn *bind_fn_;

    };

    class FontRenderer{
    public:
        void Init();
        void Destroy();

        void Begin();

        void End();

        void DrawQuad(const glm::vec3& pos, const glm::vec2& size, const glm::vec4& color);
        void DrawTexture(const Texture& texture,const glm::vec3& pos, const glm::vec2& size, const glm::vec4& color = {1,1,1,1});

    private:

        inline void DrawQuadImpl(const glm::vec3& pos, const glm::vec2& size,float texture_id, const glm::vec4& color);
    };
}


#endif //ARC_ENGINE_FONT_RENDERER_H
