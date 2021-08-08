//
// Created by adolin on 26. 12. 20.
//

#include "../include/texture.h"
#include "../../../../external/stb_image/include/image.h"
#include "../include/opengl_func.h"
#include "engine.h"
#include <glad/glad.h>
namespace arc {

void Texture::Setup2D(const std::string &file_name, const TexConf &config) {

  int width, height, channels;
  Image image;
  image.Load(Engine::config().asset_folder + file_name, &width, &height,
             &channels);
  width_ = width;
  height_ = height;
  unsigned char *data = image.get_data();
  

  GLuint data_format = GL_RGB;
  GLuint internal_format = GL_RGB8;
  if (channels == 4) {
    data_format = GL_RGBA;
    internal_format = GL_RGBA8;
  }
  arc_core_assert(channels == 3 || channels == 4, "Format not supported!");

  internal_format_ = internal_format;
  data_format_ = data_format;
  glCreateTextures(GL_TEXTURE_2D, 1, &id_);
  glTextureStorage2D(id_, 1, internal_format_, width_, height_);
  glTextureParameteri(id_, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTextureParameteri(id_, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTextureParameteri(id_, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTextureParameteri(id_, GL_TEXTURE_WRAP_T, GL_REPEAT); //TODO: expand TexConfig
  glTextureSubImage2D(id_, 0, 0, 0, width_, height_, data_format_,
                      GL_UNSIGNED_BYTE, data);
}
/* Texture::Texture(const std::string &path) { */
/*   int width, height, channels; */
/*   Image image; */
/*   unsigned char *data; */
/*   image.Load(path, &width, &height, &channels); */
/*   data = image.get_data(); */
/*   arc_core_assert(data, "Failed to load Image: {0}", path); */
/*   width_ = width; */
/*   height_ = height; */
/*   GLuint data_format = GL_RGB; */
/*   GLuint internal_format = GL_RGB8; */
/*   if (channels == 4) { */
/*     data_format = GL_RGBA; */
/*     internal_format = GL_RGBA8; */
/*   } */
/*   arc_core_assert(channels == 3 || channels == 4, "Format not supported!");
 */
/*   internal_format_ = internal_format; */
/*   data_format_ = data_format; */
/*   glCreateTextures(GL_TEXTURE_2D, 1, &id_); */
/*   glTextureStorage2D(id_, 1, internal_format_, width_, height_); */
/*   glTextureParameteri(id_, GL_TEXTURE_MIN_FILTER, GL_NEAREST); */
/*   glTextureParameteri(id_, GL_TEXTURE_MAG_FILTER, GL_NEAREST); */
/*   glTextureParameteri(id_, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); */
/*   glTextureParameteri(id_, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); */
/*   glTextureSubImage2D(id_, 0, 0, 0, width_, height_, data_format_,
 */
/*                       GL_UNSIGNED_BYTE, data); */
/* } */

/* Texture::Texture(const std::string &name, int width, int height,
 */
/*   width_ = width; */
/*   height_ = height; */
/*   data_format_ = GL_RGBA; */
/*   internal_format_ = GL_RGBA8; */
/*   glCreateTextures(GL_TEXTURE_2D, 1, &id_); */
/*   glTextureStorage2D(id_, 1, internal_format_, width_, height_); */
/*   glTextureParameteri(id_, GL_TEXTURE_MIN_FILTER, GL_NEAREST); */
/*   glTextureParameteri(id_, GL_TEXTURE_MAG_FILTER, GL_NEAREST); */
/*   glTextureParameteri(id_, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); */
/*   glTextureParameteri(id_, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); */
/*   glTextureSubImage2D(id_, 0, 0, 0, width, height, data_format_, */
/*                       GL_UNSIGNED_BYTE, data); */
/* } */

/* Texture::~Texture() { glDeleteTextures(1, &id_); } */

void Texture::Setup2D(int width, int height, void *data,
                      const TexConf &config) {
  arc_assert(!(*this), "Texture Setup failed! Texture already exists");
  width_ = width;
  height_ = height;
  depth_ = 0;

  data_format_ = GetDataFormat(config);
  internal_format_ = GetInternalFormat(config);

  glCreateTextures(GL_TEXTURE_2D, 1, &id_);
  logi("texture id is {0}", id_);
  uint interpolation =
      TexConf::NEAREST == config.interpolation ? GL_NEAREST : GL_LINEAR;
  glTextureParameteri(id_, GL_TEXTURE_MIN_FILTER, interpolation);
  glTextureParameteri(id_, GL_TEXTURE_MAG_FILTER, interpolation);

  glTextureParameteri(id_, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTextureParameteri(id_, GL_TEXTURE_WRAP_T, GL_REPEAT);

  glTextureStorage2D(id_, 1, GetSizedInternalFormat(config), width_, height_);
  glTextureSubImage2D(id_, 0, 0, 0, width_, height_, internal_format_,
                      data_format_, data);
}
void Texture::Setup3D(int width, int height, int depth, void *data,
                      const TexConf &config) {
  arc_assert(!(*this), "Texture Setup failed! Texture already exists");
  width_ = width;
  height_ = height;
  depth_ = depth;

  data_format_ = GetDataFormat(config);
  internal_format_ = GetInternalFormat(config);

  glCreateTextures(GL_TEXTURE_3D, 1, &id_);
  logi("texture id is {0}", id_);
  uint interpolation =
      TexConf::NEAREST == config.interpolation ? GL_NEAREST : GL_LINEAR;
  glTextureParameteri(id_, GL_TEXTURE_MIN_FILTER, interpolation);
  glTextureParameteri(id_, GL_TEXTURE_MAG_FILTER, interpolation);

  glTextureParameteri(id_, GL_TEXTURE_WRAP_S, GL_CLAMP);
  glTextureParameteri(id_, GL_TEXTURE_WRAP_T, GL_CLAMP);
  glTextureParameteri(id_, GL_TEXTURE_WRAP_R, GL_CLAMP);

  glBindTexture(GL_TEXTURE_3D, 0);

  glTextureStorage3D(id_, 1, GetSizedInternalFormat(config), width_, height_,
                     depth_);
  glTextureSubImage3D(id_, 0, 0, 0, 0, width_, height_, depth_,
                      internal_format_, data_format_, data);
}
void Texture::Dispose() {
  if (*this) {
    glDeleteTextures(1, &id_);
    id_ = 0;
  }
}

void Texture::set_data(void *data, int sw, int sh, int sd, int width,
                       int height, int depth) {
  GLint rl,ih,sp,sr,si;
  glGetIntegerv(GL_UNPACK_ROW_LENGTH,&rl);
  glGetIntegerv(GL_UNPACK_IMAGE_HEIGHT, &ih);
  glGetIntegerv(GL_UNPACK_SKIP_PIXELS, &sp);
  glGetIntegerv(GL_UNPACK_SKIP_ROWS, &sr);
  glGetIntegerv(GL_UNPACK_SKIP_IMAGES, &si);

  glPixelStorei(GL_UNPACK_ROW_LENGTH, width_);
  glPixelStorei(GL_UNPACK_IMAGE_HEIGHT, height_);
  glPixelStorei(GL_UNPACK_SKIP_PIXELS, sw);
  glPixelStorei(GL_UNPACK_SKIP_ROWS, sh);
  glPixelStorei(GL_UNPACK_SKIP_IMAGES, sd);


  glTextureSubImage3D(id_, 0, sw, sh, sd, width, height, depth,
                  internal_format_, data_format_, data);

  glPixelStorei(GL_UNPACK_ROW_LENGTH, rl);
  glPixelStorei(GL_UNPACK_IMAGE_HEIGHT, ih);
  glPixelStorei(GL_UNPACK_SKIP_PIXELS, sp);
  glPixelStorei(GL_UNPACK_SKIP_ROWS, sr);
  glPixelStorei(GL_UNPACK_SKIP_IMAGES, si);
}
void Texture::Bind(unsigned int slot) const { glBindTextureUnit(slot, id_); }

} // namespace arc
