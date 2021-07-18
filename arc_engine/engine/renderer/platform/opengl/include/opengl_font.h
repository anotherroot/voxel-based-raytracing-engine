//
// Created by adolin on 16. 01. 21.
//

#ifndef ARC_ENGINE_OPENGL_FONT_H
#define ARC_ENGINE_OPENGL_FONT_H
#if idk
#include "texture.h"
#include "font.h"
namespace arc{
    class OpenGLFont: public TextureClass {
    public:
        OpenGLFont(const std::string& file_name, int height);

        unsigned int width() const override{return width_;}

        unsigned int height() const override{return height_;}

        unsigned int depth() const override {return 1;}

        unsigned int renderer_id() const override{return renderer_id_;}

        void set_data(void *data, unsigned int size) override;

        void Bind(unsigned int slot) const override;

        bool Equal(const TextureClass &other) const override{
            return other.renderer_id() == renderer_id_;
        }

        ~OpenGLFont() override;

    private:
        uint width_, height_;
        uint renderer_id_;
        uint internal_format_, data_format_;
    };
}
#endif



#endif //ARC_ENGINE_OPENGL_FONT_H
