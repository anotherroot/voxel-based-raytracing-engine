//
// Created by adolin on 26. 12. 20.
//

#ifndef ARC_ENGINE_TEXTURE_H
#define ARC_ENGINE_TEXTURE_H
#include "core.h"
#include "structs.h"
namespace arc {


class TextureClass {
public:
  virtual unsigned int width() const = 0;
  virtual unsigned int height() const = 0;
  virtual unsigned int depth() const = 0;
  virtual unsigned int renderer_id() const = 0;

  virtual void set_data(void *data, unsigned int size) = 0;

  virtual void Bind(unsigned int slot = 0) const = 0;
  virtual bool Equal(const TextureClass &other) const = 0;
  virtual ~TextureClass() = default;
};
class Texture : public Caller {

  typedef uint(UintMemberFn)(void *);
  typedef void(VoidMemberVoidUintFn)(void *, void *, uint);
  typedef void(VoidMemberUintFn)(void *, uint);
  typedef bool(BoolMemberTextFn)(void *, const TextureClass &other);

  TextureClass *texture_{nullptr};
  UintMemberFn *width_fn_, *height_fn_, *depth_fn_, *renderer_id_fn_;
  VoidMemberVoidUintFn *set_data_fn_;
  VoidMemberUintFn *bind_fn_;
  BoolMemberTextFn *equal_fn_;
  inline void BindFn() {
    width_fn_ = (UintMemberFn *)GetVTable(texture_)[0];
    height_fn_ = (UintMemberFn *)GetVTable(texture_)[1];
    depth_fn_ = (UintMemberFn *)GetVTable(texture_)[2];
    renderer_id_fn_ = (UintMemberFn *)GetVTable(texture_)[3];
    set_data_fn_ = (VoidMemberVoidUintFn *)GetVTable(texture_)[4];
    bind_fn_ = (VoidMemberUintFn *)GetVTable(texture_)[5];
    equal_fn_ = (BoolMemberTextFn *)GetVTable(texture_)[6];
  }

public:
  void Create2D(const std::string &path);
  void Create2D(const std::string &name, int width, int height, void *data);
  void Create3D(int width, int height, int depth, void *data, const TextureConfig &config);

  ~Texture();
  inline void Dispose() {
    if (texture_ != nullptr) {
      delete texture_;
      texture_ = nullptr;
    }
  }
  inline unsigned int width() const { return width_fn_(texture_); }
  inline unsigned int height() const { return height_fn_(texture_); }
  inline unsigned int depth() const { return depth_fn_(texture_); }
  inline unsigned int renderer_id() const { return renderer_id_fn_(texture_); }

  inline void set_data(void *data, unsigned int size) {
    set_data_fn_(texture_, data, size);
  }

  inline void Bind(unsigned int slot = 0) const { bind_fn_(texture_, slot); }
  inline bool Equal(const TextureClass &other) const {
    return equal_fn_(texture_, other);
  }

private:
  inline TextureClass *GetTexture() const { return texture_; }
  inline VoidMemberUintFn *GetBindFunction() const { return bind_fn_; }

  friend class BatchRenderer;
};
} // namespace arc

#endif // ARC_ENGINE_TEXTURE_H
