//
// Created by adolin on 23. 12. 20.
//

#include "batch_renderer.h"
#include "renderer_api.h"
#include <glm/gtc/matrix_transform.hpp>

#include <engine.h>
#include <glm/gtx/rotate_vector.hpp>
namespace arc{

    static const uint s_maxNumQuads = 10000;
    static const uint s_maxNumTextures = 8;



    void BatchRenderer::Init(){
        vertex_array_.Create();

        vertex_buffer_.Setup(s_maxNumQuads * 4 * sizeof(Vertex));
        vertex_buffer_.set_layout({
                                          {ShaderDataType::Float3,"a_position"},
                                          {ShaderDataType::Float2,"a_tex_coord"},
                                          {ShaderDataType::Float,"a_tex_id"},
                                          {ShaderDataType::Float4,"a_color"},
        });

        vertex_array_.AddVertexBuffer(vertex_buffer_);

        vertex_array_text_.Create();
        vertex_buffer_text_.Setup(s_maxNumTextures*4*sizeof(VertexText));
        vertex_buffer_text_.set_layout({
                                               {ShaderDataType::Float2,"a_position"},
                                               {ShaderDataType::Float2,"a_tex_coord"},
                                               {ShaderDataType::Float,"a_tex_index"}
        });
        vertex_array_text_.AddVertexBuffer(vertex_buffer_text_);


        vertices_ = new Vertex[s_maxNumQuads*4];
        vertices_text_ = new VertexText[s_maxNumQuads*4];

        indices_ = new uint[s_maxNumQuads*6];

        uint vertex_index = 0;
        for (int i = 0; i < s_maxNumQuads*6; i+=6) {
            indices_[i+0] = vertex_index;
            indices_[i+1] = vertex_index+1;
            indices_[i+2] = vertex_index+2;

            indices_[i+3] = vertex_index+2;
            indices_[i+4] = vertex_index+3;
            indices_[i+5] = vertex_index;
            vertex_index+=4;
        }

        index_buffer_.Setup(indices_, s_maxNumQuads * 6);
        vertex_array_.set_index_buffer(index_buffer_);

        index_buffer_text_.Setup(indices_,s_maxNumQuads*6);
        vertex_array_text_.set_index_buffer(index_buffer_text_);

        shader_.Create("/home/tilen/Projects/diploma/impl/renderer/arc_engine/engine/shaders/2d_shader");

        int samplers[s_maxNumTextures];
        for (int i = 0; i < s_maxNumTextures; ++i) {
            samplers[i] = i;
        }
        shader_.Bind();
        shader_.SetIntArray("u_textures",samplers,s_maxNumTextures);
        shader_.Unbind();

        //shader_text_.Create("/home/martines/Desktop/git_repos/arc_engine/engine/shaders/text_shader"); // TODO: Fix on android
        //shader_text_.Create("shaders/text_shader/opengles.glsl");
//        shader_text_.Bind();
//        shader_text_.SetIntArray("u_textures",samplers,s_maxNumTextures);
//        shader_text_.Unbind();


        textures_ = new TextureCont[s_maxNumTextures];
        uint8_t white_texture_data[4] = {255,255,255,255};
        white_texture_.Create2D("white", 1,1,white_texture_data);

        textures_[0] = {white_texture_.GetTexture(), white_texture_.GetBindFunction()};

        //textures_text_ = new Font*[s_maxNumTextures];

    }

    void BatchRenderer::Destroy() {

        delete[] indices_;
        delete[] vertices_;
        delete[] textures_;
    }

    void BatchRenderer::Begin(const OrthographicCamera &camera) {
        shader_.Bind();
        shader_.SetMat4("u_view_projection",camera.view_projection());
        shader_.Unbind();

//        shader_text_.Bind();
//        shader_text_.SetMat4("u_projection",glm::ortho(0.0f, (float)Engine::window().width(), 0.0f, (float)Engine::window().height()));
//        shader_text_.SetFloat4("u_color",{1,1,1,1});
//        shader_text_.Unbind();
        num_quads_ = 0;
        num_textures_ = 1;
        num_quads_text_ = 0;
        num_textures_text_ = 0;
    }

    void BatchRenderer::End() {


        Flush();

//        shader_text_.Bind();
//        vertex_array_text_.Bind();
//        for (int i = 0; i < num_textures_text_; ++i) {
//            textures_text_[i]->Bind(i);
//
//        }
//        vertex_buffer_text_.SetData(vertices_text_, num_quads_text_ * 4 * sizeof(VertexText));
//        vertex_array_text_.set_index_buffer(index_buffer_text_);
//        RenderCommand::DrawIndexed(vertex_array_text_,num_quads_text_*6);
//
//
//        vertex_array_text_.Unbind();
//        shader_text_.Unbind();
    }

    void BatchRenderer::Flush() {

        shader_.Bind();
        vertex_array_.Bind();
        for (int i = 0; i < num_textures_; ++i) {
            textures_[i].bind_fn_(textures_[i].object,i);
        }
        vertex_buffer_.SetData(vertices_, num_quads_ * 4 * sizeof(Vertex));
        vertex_array_.set_index_buffer(index_buffer_);
        RenderCommand::DrawIndexed(vertex_array_,num_quads_*6);

        vertex_array_.Unbind();
        shader_.Unbind();


        num_quads_ = 0;
        num_textures_ = 1;
        num_quads_text_ = 0;
        num_textures_text_ = 0;
    }

    void BatchRenderer::DrawQuad(const glm::vec3 &pos, const glm::vec2 &size, const glm::vec4 &color,float rotation) {

        if(num_quads_ == s_maxNumQuads){
            Flush();
        }

        DrawRotatedQuadImpl(pos, size,rotation, 0,color);
    }



    void BatchRenderer::DrawQuad(const glm::vec3 &pos, const glm::vec2 &size, const glm::vec4 &color) {
        if(num_quads_ == s_maxNumQuads){
            Flush();
        }
        DrawQuadImpl(pos,size,0,color);
    }

    void BatchRenderer::DrawTexture(const Texture &texture, const glm::vec3 &pos, const glm::vec2 &size,float rotation,
                                    const glm::vec4 &color) {
        if(num_quads_ == s_maxNumQuads){
            Flush();
        }
        int texture_id=  0;
        TextureClass* object = texture.GetTexture();
        for (int i = 0; i < num_textures_; ++i) {
            if(object == textures_[i].object){
                texture_id = i;
            }
        }
        if(texture_id == 0){
            if(num_textures_ == s_maxNumTextures){
                Flush();
            }
            textures_[num_textures_].object = object;
            textures_[num_textures_].bind_fn_ = texture.GetBindFunction();
            texture_id = num_textures_++;
        }

        DrawRotatedQuadImpl(pos,size,rotation,(float)texture_id,color);

    }

    void BatchRenderer::DrawTexture(const Texture &texture, const glm::vec3 &pos, const glm::vec2 &size,
                                    const glm::vec4 &color) {
        if(num_quads_ == s_maxNumQuads){
            Flush();
        }
        int texture_id=  0;
        TextureClass* object = texture.GetTexture();
        for (int i = 0; i < num_textures_; ++i) {
            if(object == textures_[i].object){
                texture_id = i;
            }
        }
        if(texture_id == 0){
            if(num_textures_ == s_maxNumTextures){
                Flush();
            }
            textures_[num_textures_].object = object;
            textures_[num_textures_].bind_fn_ = texture.GetBindFunction();
            texture_id = num_textures_++;
        }

        DrawQuadImpl(pos,size,(float)texture_id,color);
    }

    inline void SetQuadData(Vertex* vertices_,int texture_id, const glm::vec4& color){
        vertices_[0].tex_coord = {0,0};
        vertices_[1].tex_coord = {1,0};
        vertices_[2].tex_coord = {1,1};
        vertices_[3].tex_coord = {0,1};

        vertices_[0].tex_id = texture_id;
        vertices_[1].tex_id = texture_id;
        vertices_[2].tex_id = texture_id;
        vertices_[3].tex_id = texture_id;

        vertices_[0].color = color;
        vertices_[1].color = color;
        vertices_[2].color = color;
        vertices_[3].color = color;
    }

    void BatchRenderer::DrawRotatedQuadImpl(const glm::vec3 &pos, const glm::vec2 &size,float rotation, float texture_id,
                                            const glm::vec4 &color) {
        glm::vec2 o = size / 2.0f;

        glm::vec3 points[4];

        points[0] = glm::vec3(-o.x, -o.y, 0);
        points[1]= glm::vec3(o.x, -o.y, 0);
        points[2] = glm::vec3(o.x, o.y, 0);
        points[ 3] = glm::vec3(-o.x, o.y, 0);

        //rotate



        for (int i = 0; i < 4; ++i) {
            points[i] = glm::rotate(points[i],glm::radians(rotation),{0,0,1});
            vertices_[num_quads_*4+i].pos = pos + points[i];
        }

        SetQuadData(&vertices_[num_quads_*4],texture_id,color);
        num_quads_++;

    }

    void BatchRenderer::DrawQuadImpl(const glm::vec3 &pos, const glm::vec2 &size, float texture_id,
                                     const glm::vec4 &color) {
        glm::vec2 o = size / 2.0f;
        vertices_[num_quads_*4+0].pos = pos + glm::vec3(-o.x, -o.y, 0);
        vertices_[num_quads_*4+1].pos = pos + glm::vec3(o.x, -o.y, 0);
        vertices_[num_quads_*4+2].pos = pos + glm::vec3(o.x, o.y, 0);
        vertices_[num_quads_*4+3].pos = pos + glm::vec3(-o.x, o.y, 0);

        SetQuadData(&vertices_[num_quads_*4],texture_id,color);
        num_quads_++;
    }


//    void BatchRenderer::DrawScreenText( Font &font, const std::string &text, const glm::vec2 &poss) {
//        if(num_quads_text_ == s_maxNumQuads){
//            return;
//        }
//        int texture_id = 0;
//        for (int i = 0; i < num_textures_text_; ++i) {
//            if(&font == textures_text_[i]){
//                texture_id = i;
//            }
//        }
//        if(texture_id == 0){
//            if(num_textures_text_ == s_maxNumTextures){
//                return;
//            }
//            textures_text_[num_textures_text_] = &font;
//            texture_id = num_textures_text_++;
//        }
//
//
//        std::string::const_iterator c;
//        float scale = 1.0f;
//        float x = poss.x;
//        float width = 0;
//        float height = 0;
//        for (c = text.begin(); c != text.end(); c++) {
//            Character ch = font.characters_.at(*c);
//            width += ch.bl * scale;
//            width += ch.ax  * scale;
//        }
//        for (c = text.begin(); c != text.end(); c++)
//        {
//            if(num_quads_text_ == s_maxNumQuads){
//                return;
//            }
//            Character ch = font.characters_.at(*c);
//
//            float x_pos = x + ch.bl * scale;
//            float y_pos = poss.y - (ch.bh - ch.bt) * scale;
//
//            float w = ch.bw * scale;
//            float h = ch.bh * scale;
//            // update VBO for each character
//
//            vertices_text_[num_quads_text_*4].pos = {x_pos, y_pos};
//            vertices_text_[num_quads_text_*4 + 1].pos = {x_pos + w, y_pos };
//            vertices_text_[num_quads_text_*4 + 2].pos = {x_pos + w, y_pos + h};
//            vertices_text_[num_quads_text_*4 + 3].pos = {x_pos, y_pos+ h};
//
//            vertices_text_[num_quads_text_*4].tex_coord = {ch.tx - (float)ch.bw/font.width_,(float)ch.bh/font.height_};
//            vertices_text_[num_quads_text_*4 + 1].tex_coord  = {ch.tx,(float)ch.bh/font.height_};
//            vertices_text_[num_quads_text_*4 + 2].tex_coord  = {ch.tx,0};
//            vertices_text_[num_quads_text_*4 + 3].tex_coord = {ch.tx - (float)ch.bw/font.width_,0};
//
//
//            vertices_text_[num_quads_text_*4].tex_id = (float)texture_id;
//            vertices_text_[num_quads_text_*4 + 1].tex_id  = (float)texture_id;
//            vertices_text_[num_quads_text_*4 + 2].tex_id  = (float)texture_id;
//            vertices_text_[num_quads_text_*4 + 3].tex_id = (float)texture_id;
//
//            num_quads_text_++;
//
//            x += ch.ax  * scale;
//        }
//
//
//    }
//
//    void BatchRenderer::DrawWorldText(const Font &font, const std::string &text, const glm::vec3 &pos) {
//
//    }
}
