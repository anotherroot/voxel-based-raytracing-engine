//
// Created by adolin on 26. 12. 20.
//

#include "../include/opengl_texture.h"
#include "../../../../../external/stb_image/include/image.h"
#include "opengl_func.h"
#include <glad/glad.h>
namespace arc {

OpenGLTexture::OpenGLTexture(const std::string &path) {
  int width, height, channels;

  Image image;
  unsigned char *data;

  image.Load(path, &width, &height, &channels);
  data = image.get_data();

  arc_core_assert(data, "Failed to load Image: {0}", path);

  width_ = width;
  height_ = height;
  GLuint data_format = GL_RGB;
  GLuint internal_format = GL_RGB8;
  if (channels == 4) {
    data_format = GL_RGBA;
    internal_format = GL_RGBA8;
  }
  arc_core_assert(channels == 3 || channels == 4, "Format not supported!");

  internal_format_ = internal_format;
  data_format_ = data_format;

  glCreateTextures(GL_TEXTURE_2D, 1, &renderer_id_);
  glTextureStorage2D(renderer_id_, 1, internal_format_, width_, height_);

  glTextureParameteri(renderer_id_, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTextureParameteri(renderer_id_, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  glTextureParameteri(renderer_id_, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTextureParameteri(renderer_id_, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  glTextureSubImage2D(renderer_id_, 0, 0, 0, width_, height_, data_format_,
                      GL_UNSIGNED_BYTE, data);
}

OpenGLTexture::OpenGLTexture(const std::string &name, int width, int height,
                             void *data) {
  width_ = width;
  height_ = height;
  data_format_ = GL_RGBA;
  internal_format_ = GL_RGBA8;

  glCreateTextures(GL_TEXTURE_2D, 1, &renderer_id_);
  glTextureStorage2D(renderer_id_, 1, internal_format_, width_, height_);

  glTextureParameteri(renderer_id_, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTextureParameteri(renderer_id_, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  glTextureParameteri(renderer_id_, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTextureParameteri(renderer_id_, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  glTextureSubImage2D(renderer_id_, 0, 0, 0, width, height, data_format_,
                      GL_UNSIGNED_BYTE, data);
}

OpenGLTexture::~OpenGLTexture() { glDeleteTextures(1, &renderer_id_); }

void OpenGLTexture::set_data(void *data, unsigned int size) {
  unsigned int bpc = data_format_ == GL_RGBA ? 4 : 3;
  arc_core_assert(size == width_ * height_ * bpc,
                  "Data must be entire texture!");
  glBindTexture(GL_TEXTURE_2D, renderer_id_);
  glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width_, height_, data_format_,
                  GL_UNSIGNED_BYTE, data);
}

void OpenGLTexture::Bind(unsigned int slot) const {
  glBindTextureUnit(slot, renderer_id_);
}

OpenGLTexture3D::OpenGLTexture3D(int width, int height, int depth, void *data,
                                 const TextureConfig &config)
    : width_(width), height_(height), depth_(depth) {

  data_format_ = GetDataFormat(config);
  internal_format_ = GetInternalFormat(config);

  glCreateTextures(GL_TEXTURE_3D, 1, &renderer_id_);

  uint interpolation =
      TextureConfig::NEAREST == config.interpolation ? GL_NEAREST : GL_LINEAR;
  glTextureParameteri(renderer_id_, GL_TEXTURE_MIN_FILTER, interpolation);
  glTextureParameteri(renderer_id_, GL_TEXTURE_MAG_FILTER, interpolation);

  glTextureParameteri(renderer_id_, GL_TEXTURE_WRAP_S, GL_CLAMP);
  glTextureParameteri(renderer_id_, GL_TEXTURE_WRAP_T, GL_CLAMP);
  glTextureParameteri(renderer_id_, GL_TEXTURE_WRAP_R, GL_CLAMP);

  glTextureStorage3D(renderer_id_, 1, GetSizedInternalFormat(config), width_,
                     height_, depth_);
  glTextureSubImage3D(renderer_id_, 0, 0, 0, 0, width_, height_, depth_,
                      internal_format_, data_format_, data);
}
OpenGLTexture3D::~OpenGLTexture3D() { glDeleteTextures(1, &renderer_id_); }

void OpenGLTexture3D::set_data(void *data, unsigned int size) {
  glBindTexture(GL_TEXTURE_3D, renderer_id_);
  glTexSubImage3D(GL_TEXTURE_3D, 0, 0, 0, 0, width_, height_, depth_,
                      internal_format_, data_format_, data);
  glBindTexture(GL_TEXTURE_3D, 0);
}
void OpenGLTexture3D::Bind(unsigned int slot) const {
  glBindTextureUnit(slot, renderer_id_);
}

} // namespace arc
