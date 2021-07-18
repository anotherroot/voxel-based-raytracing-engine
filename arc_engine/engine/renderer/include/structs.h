#ifndef ARC_ENGINE_STRUCTS_H
#define ARC_ENGINE_STRUCTS_H
#include "core.h"
namespace arc {
struct TextureConfig {
  enum InternalFormat { R, RG, RGB, RGBA, DEPTH, STENCIL };
  enum DataFormat {
    UNSIGNED_BYTE,
    BYTE,
    UNSIGNED_SHORT,
    SHORT,
    UNSIGNED_INT,
    INT,
    FLOAT
  };
  enum Interpolation { NEAREST, LINEAR };

  InternalFormat internal_format;
  DataFormat data_format;
  Interpolation interpolation;
};
struct FrameBufferConfig {
  std::vector<TextureConfig> frames;
  FrameBufferConfig(uint w,uint h,uint samp,const std::initializer_list<TextureConfig> &f, bool ds) : frames(f), width(w), height(h), samples(samp), depth_stencil(ds) {}
  uint width, height;
  uint samples;
  bool depth_stencil;
};
}; // namespace arc

#endif // ARC_ENGINE_STRUCTS_H
