//
// Created by adolin on 3. 01. 21.
//

#include "glad/glad.h"
#include "font.h"

#include "ft2build.h"
#include FT_FREETYPE_H
namespace arc{

    void Font::Create(const std::string& file_name, int font_height) {
        Dispose();
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
        FT_Set_Pixel_Sizes(face, 0, font_height);



        //glPixelStorei(GL_UNPACK_ALIGNMENT,1);

        width_ = 0; height_ = 0;
        for (unsigned char c = 32; c < 128; c++) {
            // load character glyph
            if (int error = FT_Load_Char(face, c, FT_LOAD_RENDER)) {
                logce("ERROR::FREETYTPE: Failed to load Glyph ({}), error: {}", c, error);
                continue;
            }
            auto g = face->glyph;
            width_ += g->bitmap.width;
            if(g->bitmap.rows > height_){
                height_ = g->bitmap.rows;
            }
        }

        glActiveTexture(GL_TEXTURE0);
        glGenTextures(1, &render_id_);
        glBindTexture(GL_TEXTURE_2D, render_id_);
        glPixelStorei(GL_UNPACK_ALIGNMENT,1);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width_, height_, 0,GL_RED, GL_UNSIGNED_BYTE, 0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);


        int x = 0;

        for(unsigned char c = 32; c < 128; c++) {
            if(FT_Load_Char(face, c, FT_LOAD_RENDER))
                continue;
            auto g = face->glyph;

            glTexSubImage2D(GL_TEXTURE_2D, 0, x, 0, g->bitmap.width, g->bitmap.rows, GL_RED, GL_UNSIGNED_BYTE, g->bitmap.buffer);

            x += g->bitmap.width;
            characters_[c] = {};
            characters_[c].ax = g->advance.x >> 6;
            characters_[c].ay = g->advance.y >> 6;

            characters_[c].bw = g->bitmap.width;
            characters_[c].bh = g->bitmap.rows;

            characters_[c].bl = g->bitmap_left;
            characters_[c].bt = g->bitmap_top;

            characters_[c].tx = (float)x / (float)width_;
        }


        //cleaning up
        FT_Done_Face(face);
        FT_Done_FreeType(ft);

    }



    Font::~Font() {
        Dispose();
    }

    void Font::Bind(int slot) {
        glBindTextureUnit(slot, render_id_);
    }
}

