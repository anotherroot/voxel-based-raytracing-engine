//

#ifndef ARC_ENGINE_BATCH_RENDERER_H
#define ARC_ENGINE_BATCH_RENDERER_H

#include <glm.hpp>
#include "orthographic_camera.h"
#include "vertex_array.h"
#include "shader.h"
#include "texture.h"

namespace arc{
    struct Vertex{
        glm::vec3 pos;
        glm::vec2 tex_coord;
        float tex_id;
        glm::vec4 color;
    };
    struct VertexText{
        glm::vec2 pos;
        glm::vec2 tex_coord;
        float tex_id;
    };
    struct TextureCont{
        typedef void(VoidMemberUintFn)(void*,uint);
        void* object;
        VoidMemberUintFn *bind_fn_;

    };
    enum FontSpace{
        WORLD, SCREEN
    };
    class BatchRenderer{

    public:
        void Init();
        void Destroy();
        void Begin(const arc::OrthographicCamera &camera);

        void End();

        void DrawQuad(const glm::vec3& pos, const glm::vec2& size, const glm::vec4& color,float rotation);
        void DrawQuad(const glm::vec3& pos, const glm::vec2& size, const glm::vec4& color);
        void DrawTexture(const Texture& texture,const glm::vec3& pos, const glm::vec2& size,float rotation, const glm::vec4& color = {1,1,1,1});
        void DrawTexture(const Texture& texture,const glm::vec3& pos, const glm::vec2& size, const glm::vec4& color = {1,1,1,1});

//        inline void DrawText(Font& font,const std::string& text, const glm::vec3&pos, const FontSpace space = SCREEN){
//            switch (space) {
//                case SCREEN:
//                    DrawScreenText(font,text,pos);
//                    break;
//                case WORLD:
//                    DrawWorldText(font,text,pos);
//                    break;
//            }
//        }
//        void DrawScreenText(Font& font,const std::string& text, const glm::vec2&poss);
//        void DrawWorldText(const Font& font,const std::string& text, const glm::vec3&pos);

    private:
        void Flush();
        void DrawRotatedQuadImpl(const glm::vec3& pos, const glm::vec2& size,float rotation, float texture_id, const glm::vec4& color);
        void DrawQuadImpl(const glm::vec3& pos, const glm::vec2& size, float texture_id, const glm::vec4& color);


        Shader shader_;

        Shader shader_text_;
        Texture white_texture_;


        VertexArray vertex_array_;

        IndexBuffer index_buffer_;
        VertexBuffer vertex_buffer_;

        uint num_quads_;
        Vertex* vertices_;
        uint* indices_;

        TextureCont* textures_;
        uint num_textures_;
    };


}


#endif //ARC_ENGINE_BATCH_RENDERER_H
