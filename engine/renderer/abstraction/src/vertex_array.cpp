//
// Created by adolin on 23. 12. 20.
//

#include "../include/vertex_array.h"
#include <glad/glad.h>
namespace arc {
static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type) {
  switch (type) {
  case ShaderDataType::Float:
    return GL_FLOAT;
  case ShaderDataType::Float2:
    return GL_FLOAT;
  case ShaderDataType::Float3:
    return GL_FLOAT;
  case ShaderDataType::Float4:
    return GL_FLOAT;
  case ShaderDataType::Mat3:
    return GL_FLOAT_MAT3;
  case ShaderDataType::Mat4:
    return GL_FLOAT_MAT4;
  case ShaderDataType::Int:
    return GL_INT;
  case ShaderDataType::Int2:
    return GL_INT;
  case ShaderDataType::Int3:
    return GL_INT;
  case ShaderDataType::Int4:
    return GL_INT;
  case ShaderDataType::Bool:
    return GL_BOOL;
  }
}

void VertexArray::Setup() {
  arc_assert(!(*this), "VertexArray Setup failed! VertexArray already exists");
  glGenVertexArrays(1, &id_);
  logi("vertex array setup {0}",id_);
}

void VertexArray::Dispose() {
  if (*this) {
    glDeleteVertexArrays(1, &id_);
    id_ = 0;
  }
}

void VertexArray::Bind() const { glBindVertexArray(id_); }

void VertexArray::Unbind() const { glBindVertexArray(0); }

void VertexArray::BindVertexBuffer(const VertexBuffer &vertex_buffer) {
  glBindVertexArray(id_);
  vertex_buffer.Bind();

  arc_core_assert(vertex_buffer.layout().elements().size(),
                  "Vertex Buffer has NO Layout!");

  uint index = 0;
  for (const auto &element : vertex_buffer.layout()) {
    glEnableVertexAttribArray(index);
    glVertexAttribPointer(index, element.GetComponentCount(),
                          ShaderDataTypeToOpenGLBaseType(element.type),
                          element.normalized ? GL_TRUE : GL_FALSE,
                          vertex_buffer.layout().stride(),
                          (const void *)element.offset);
    index++;
  }
}

void VertexArray::BindIndexBuffer(const IndexBuffer &index_buffer) {
  glBindVertexArray(id_);
  index_buffer.Bind();
}
} // namespace arc
