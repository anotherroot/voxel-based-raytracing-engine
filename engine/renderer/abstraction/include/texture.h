//
// Created by adolin on 26. 12. 20.
//
#ifndef ARC_ENGINE_OPENGL_TEXTURE_H
#define ARC_ENGINE_OPENGL_TEXTURE_H

#include "structs.h"
namespace arc {

class Texture {
public:
  static void Bind(int id, unsigned int slot);


  void Setup2D(const std::string& file_name,
             const TexConf &config);
  void Setup2D(int width, int height, void *data,
             const TexConf &config);
  void Setup3D(int width, int height, int depth, void *data,
             const TexConf &config);
  void Dispose();
  ~Texture() { Dispose(); }
  operator bool() const { return id_; }

  unsigned int width() const { return width_; }

  unsigned int height() const { return height_; }

  unsigned int depth() const { return depth_; }
  unsigned int renderer_id() const { return id_; };

  void set_data(void *data, int sw, int sh, int sd, int width, int height, int depth);

  void Bind(unsigned int slot) const;
  bool Equal(const Texture &other) const { return id_ == other.renderer_id(); }

private:
  uint width_, height_, depth_;
  uint id_{0};
  uint internal_format_, data_format_;
};
} // namespace arc

#endif // ARC_ENGINE_OPENGL_TEXTURE_H
