#ifndef ARC_ENGINE_OPENGL_FUNC_H
#define ARC_ENGINE_OPENGL_FUNC_H
#include "core.h"
#include "glad/glad.h"
#include "structs.h"
namespace arc {
inline uint GetDataFormat(const TexConf &config) {
  switch (config.data_format) {
  case TexConf::DataFormat::UNSIGNED_BYTE:
    return GL_UNSIGNED_BYTE;
    break;
  case TexConf::DataFormat::BYTE:
    return GL_BYTE;
    break;
  case TexConf::DataFormat::UNSIGNED_SHORT:
    return GL_UNSIGNED_SHORT;
    break;
  case TexConf::DataFormat::SHORT:
    return GL_SHORT;
    break;
  case TexConf::DataFormat::UNSIGNED_INT:
    return GL_UNSIGNED_INT;
    break;
  case TexConf::DataFormat::INT:
    return GL_INT;
    break;
  case TexConf::DataFormat::FLOAT:
    return GL_FLOAT;
    break;
  }
}
inline uint GetSizedInternalFormat(const TexConf &config) {
  int index;
  switch (config.data_format) {
  case TexConf::DataFormat::UNSIGNED_BYTE:
    index = 0;
    break;
  case TexConf::DataFormat::BYTE:
    index = 0;
    break;
  case TexConf::DataFormat::UNSIGNED_SHORT:
    index = 1;
    break;
  case TexConf::DataFormat::SHORT:
    index = 1;
    break;
  case TexConf::DataFormat::UNSIGNED_INT:
    index = 2;
    break;
  case TexConf::DataFormat::INT:
    index = 2;
    break;
  case TexConf::DataFormat::FLOAT:
    index = 3;
    break;
  }
  switch (config.internal_format) {
  case TexConf::InternalFormat::R: {
    uint options[4] = {GL_R8, GL_R16, GL_R32I, GL_R32F};
    return options[index];
    break;
  }
  case TexConf::InternalFormat::RG: {
    uint options[4] = {GL_RG8, GL_RG16, GL_RG32I, GL_RG32F};
    return options[index];
    break;
  }
  case TexConf::InternalFormat::RGB: {
    uint options[4] = {GL_RGB8, GL_RGB16, GL_RGB32I, GL_RGB32F};
    return options[index];
    break;
  }
  case TexConf::InternalFormat::RGBA: {
    uint options[4] = {GL_RGBA8, GL_RGBA16, GL_RGBA32I, GL_RGBA32F};
    return options[index];
    break;
  }
  case TexConf::InternalFormat::DEPTH: {
    return 0; // TODO: are you ever gonna use this
    break;
  }
  case TexConf::InternalFormat::STENCIL: {
    uint options[4] = {GL_STENCIL_INDEX8,
                       GL_STENCIL_INDEX16}; // TODO: check what stensil is
    return 0;
  }
  }
  return 0;
}
inline uint GetInternalFormat(const TexConf &config) {
  switch (config.internal_format) {
  case TexConf::InternalFormat::R:
    return GL_RED;
  case TexConf::InternalFormat::RG:
    return GL_RG;
  case TexConf::InternalFormat::RGB:
    return GL_RGB;
  case TexConf::InternalFormat::RGBA:
    return GL_RGBA;
  case TexConf::InternalFormat::DEPTH:
    return GL_DEPTH_COMPONENT;
  case TexConf::InternalFormat::STENCIL:
    return GL_STENCIL_INDEX;
  }
  return 0;
}
inline uint GetInterpolation(const TexConf &config) {
  switch (config.interpolation) {
  case TexConf::LINEAR:
    return GL_LINEAR;
  case TexConf::NEAREST:
    return GL_NEAREST;
  }
  return 0;
}
} // namespace arc
#endif // ARC_ENGINE_OPENGL_FUNC_H
