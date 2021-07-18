//
// Created by adolin on 16. 01. 21.
//
#if idk
#include "opengl_font.h"
#include "glad/glad.h"
#include "ft2build.h"
#include FT_FREETYPE_H
namespace arc{

    OpenGLFont::OpenGLFont(const std::string &file_name, int height) {
        FT_Library ft;
        if (FT_Init_FreeType(&ft))
        {
            arc_core_assert(false,"ERROR::FREETYPE: Could not init FreeType Library");
        }

        FT_Face face;
        if (FT_New_Face(ft, file_name.c_str(), 0, &face))
        {
            arc_core_assert(false,"ERROR::FREETYPE: Failed to load font");
        }
        FT_Set_Pixel_Sizes(face, 0, height);


        data_format_ = GL_R;
        internal_format_ = GL_R8;

        //glPixelStorei(GL_UNPACK_ALIGNMENT,1);


        for (unsigned char c = 0; c < 128; c++)
        {
            // load character glyph
            if (int error = FT_Load_Char(face, c, FT_LOAD_RENDER))
            {
                logce("ERROR::FREETYTPE: Failed to load Glyph ({}), error: {}",c, error);
                continue;
            }
            // generate texture
            unsigned int texture;
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D(
                    GL_TEXTURE_2D,
                    0,
                    GL_RED,
                    face->glyph->bitmap.width,
                    face->glyph->bitmap.rows,
                    0,
                    GL_RED,
                    GL_UNSIGNED_BYTE,
                    face->glyph->bitmap.buffer
            );
            // set texture options
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            // now store character for later use
            Character character = {
                    texture,
                    glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                    glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                    face->glyph->advance.x
            };
            //characters_.insert(std::pair<char, Character>(c, character));
        }
        //cleaning up
        FT_Done_Face(face);
        FT_Done_FreeType(ft);
    }



    void OpenGLFont::set_data(void *data, unsigned int size) {
        arc_core_assert(size == width_*height_, "Data must be entire texture!");
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width_, height_,data_format_,GL_UNSIGNED_BYTE, data);

    }

    void OpenGLFont::Bind(unsigned int slot) const {
        glBindTextureUnit(slot, renderer_id_);
    }


    OpenGLFont::~OpenGLFont() {
        glDeleteTextures(1, &renderer_id_);
    }
}
#endif