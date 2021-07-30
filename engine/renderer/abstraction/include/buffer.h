//
// Created by adolin on 23. 12. 20.
//

#ifndef ARC_ENGINE_BUFFER_H
#define ARC_ENGINE_BUFFER_H
#include "structs.h"
namespace arc {
enum class ShaderDataType {
  None = 0,
  Float,
  Float2,
  Float3,
  Float4,
  Mat3,
  Mat4,
  Int,
  Int2,
  Int3,
  Int4,
  Bool
};

static uint ShaderDataTypeSize(ShaderDataType type) {
  switch (type) {
  case ShaderDataType::Float:
    return 4;
  case ShaderDataType::Float2:
    return 4 * 2;
  case ShaderDataType::Float3:
    return 4 * 3;
  case ShaderDataType::Float4:
    return 4 * 4;
  case ShaderDataType::Mat3:
    return 4 * 3 * 3;
  case ShaderDataType::Mat4:
    return 4 * 4 * 4;
  case ShaderDataType::Int:
    return 4;
  case ShaderDataType::Int2:
    return 4 * 2;
  case ShaderDataType::Int3:
    return 4 * 3;
  case ShaderDataType::Int4:
    return 4 * 4;
  case ShaderDataType::Bool:
    return 1;
  }
  arc_core_assert(false, "Unknown ShaderDataType!");
}

struct BufferElement {
  std::string name;
  ShaderDataType type;
  uint size;
  uint offset;
  bool normalized;

  BufferElement(ShaderDataType _type, const std::string _name,
                bool _normalized = false)
      : name(_name), type(_type), size(ShaderDataTypeSize(_type)), offset(0),
        normalized(_normalized) {}

  uint GetComponentCount() const {
    switch (type) {
    case ShaderDataType::Float:
      return 1;
    case ShaderDataType::Float2:
      return 1 * 2;
    case ShaderDataType::Float3:
      return 1 * 3;
    case ShaderDataType::Float4:
      return 1 * 4;
    case ShaderDataType::Mat3:
      return 1 * 3 * 3;
    case ShaderDataType::Mat4:
      return 1 * 4 * 4;
    case ShaderDataType::Int:
      return 1;
    case ShaderDataType::Int2:
      return 1 * 2;
    case ShaderDataType::Int3:
      return 1 * 3;
    case ShaderDataType::Int4:
      return 1 * 4;
    case ShaderDataType::Bool:
      return 1;
    }
    arc_core_assert(false, "Unknown ShaderDataType!");
  }
};

class BufferLayout {
public:
  BufferLayout(const std::initializer_list<BufferElement> &elements)
      : elements_(elements) {
    CalculateOffsetAndStride();
  }

  inline const std::vector<BufferElement> &elements() const {
    return elements_;
  }

  inline uint stride() const { return stride_; }

  std::vector<BufferElement>::iterator begin() { return elements_.begin(); }
  std::vector<BufferElement>::iterator end() { return elements_.end(); }

  std::vector<BufferElement>::const_iterator begin() const {
    return elements_.begin();
  }
  std::vector<BufferElement>::const_iterator end() const {
    return elements_.end();
  }

private:
  void CalculateOffsetAndStride() {
    stride_;
    for (auto &element : elements_) {
      element.offset = stride_;
      stride_ += element.size;
    }
  }

private:
  std::vector<BufferElement> elements_;
  uint stride_;
};

class VertexBuffer {
public:
  void Setup(uint size);
  void Setup(float *vertices, uint count);
  ~VertexBuffer() { Dispose(); }
  void Dispose();
  operator bool() const {return id_;}

  void Bind() const;

  void Unbind() const;

  void set_layout(const BufferLayout &layout){layout_ = layout;}
  const BufferLayout &layout() const{return layout_;}

  void SetData(const void *data, uint size);

private:
  BufferLayout layout_{};
  uint id_{0};
};

class IndexBuffer {
public:
  void Setup(uint size);
  void Setup(uint *indices, uint count);
  void Dispose();
  ~IndexBuffer() {Dispose();}
  operator bool() const {return id_;}

  void Bind() const;

  void Unbind() const;

  uint count() const{return count_;}

  void SetData(uint *indices, uint count);
private:
  uint id_{0};
  uint count_;
};
}
#endif // ARC_ENGINE_BUFFER_H
