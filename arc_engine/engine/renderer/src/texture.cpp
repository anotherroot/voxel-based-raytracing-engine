//
// Created by adolin on 26. 12. 20.
//

#include "texture.h"
#include "renderer_api.h"

#ifdef ARC_PLATFORM_OPENGL
#include "opengl_texture.h"
#elif ARC_PLATFORM_OPENGL_ES
#include "opengl_es_texture.h"
#endif
#include "engine.h"
namespace arc {

void Texture::Create2D(const std::string &path) {
  Dispose();
#ifdef ARC_PLATFORM_OPENGL
  texture_ = new OpenGLTexture(Engine::config().asset_folder + path);
#elif ARC_PLATFORM_OPENGL_ES
  texture_ = new OpenGLESTexture(Engine::config().asset_folder + path);
#else
  arc_core_assert(false, "Not Supported");
#endif

  BindFn();
}

void Texture::Create2D(const std::string &name, int width, int height,
                       void *data) {
  Dispose();
#ifdef ARC_PLATFORM_OPENGL
  texture_ = new OpenGLTexture(name, width, height, data);
#elif ARC_PLATFORM_OPENGL_ES
  texture_ = new OpenGLESTexture(name, width, height, data);
#else
  arc_core_assert(false, "Not Supported");
#endif
  BindFn();
}

Texture::~Texture() { Dispose(); }

void Texture::Create3D(int width, int height, int depth, void *data, const TextureConfig &config) {
  Dispose();
#ifdef ARC_PLATFORM_OPENGL
  texture_ = new OpenGLTexture3D(width, height, depth, data, config);
#elif ARC_PLATFORM_OPENGL_ES
  texture_ = new OpenGLESTexture3D(name, width, height, data);
#else
  arc_core_assert(false, "Not Supported");
#endif
  BindFn();
}
} // namespace arc
