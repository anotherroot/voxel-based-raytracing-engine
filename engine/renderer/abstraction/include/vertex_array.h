//
// Created by adolin on 23. 12. 20.
//

#ifndef ARC_ENGINE_OPENGL_VERTEX_ARRAY_H
#define ARC_ENGINE_OPENGL_VERTEX_ARRAY_H
#include "buffer.h"
namespace arc {
class VertexArray {
public:
  void Setup();
  void Dispose();
  ~VertexArray(){Dispose();}
  operator bool() const {return id_;}

  void Bind() const;

  void Unbind() const;

  void BindVertexBuffer(const VertexBuffer &vertex_buffer);

  void BindIndexBuffer(const IndexBuffer &index_buffer);

private:
  uint id_;
};
} // namespace arc

#endif // ARC_ENGINE_OPENGL_VERTEX_ARRAY_H
