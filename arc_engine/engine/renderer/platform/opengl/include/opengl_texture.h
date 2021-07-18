//
// Created by adolin on 26. 12. 20.
//

#ifndef ARC_ENGINE_OPENGL_TEXTURE_H
#define ARC_ENGINE_OPENGL_TEXTURE_H

#include "texture.h"
#include "structs.h"
namespace arc{
    class OpenGLTexture: public TextureClass{
    public:
        OpenGLTexture(const std::string &path);
        OpenGLTexture(const std::string &name, int width, int height, void *data);
        ~OpenGLTexture() override;

        unsigned int width() const override { return width_; }

        unsigned int height() const override { return height_; }

        unsigned int depth() const override {return 1;}

        unsigned int renderer_id() const override {return renderer_id_;};

        void set_data(void *data, unsigned int size) override;

        void Bind(unsigned int slot) const override;

        bool Equal(const TextureClass &other) const override{return renderer_id_ == other.renderer_id();}

    private:
        uint width_, height_;
        uint renderer_id_;
        uint internal_format_, data_format_;

    };
    class OpenGLTexture3D: public TextureClass{
    public:
        OpenGLTexture3D( int width, int height, int depth, void *data, const TextureConfig& config);
        ~OpenGLTexture3D() override;

        unsigned int width() const override { return width_; }

        unsigned int height() const override { return height_; }

        unsigned int depth() const override {return depth_;}

        unsigned int renderer_id() const override {return renderer_id_;};

        void set_data(void *data, unsigned int size) override;

        void Bind(unsigned int slot) const override;

        bool Equal(const TextureClass &other) const override{return renderer_id_ == other.renderer_id();}

    private:
        uint width_, height_, depth_;
        uint renderer_id_;
        uint internal_format_, data_format_;

    };
}


#endif //ARC_ENGINE_OPENGL_TEXTURE_H
