//
// Created by adolin on 23. 12. 20.
//

#include "buffer.h"
#include "renderer_api.h"
#include <glad/glad.h>
namespace arc {

void VertexBuffer::Setup(uint size) {
  arc_assert(!*this, "VertexBuffer Setup failed! VertexBuffer already exists");

  glCreateBuffers(1, &id_);
  glBindBuffer(GL_ARRAY_BUFFER, id_);
  glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
}
void VertexBuffer::Setup(float *vertices, uint size) {
  arc_assert(!*this, "VertexBuffer Setup failed! VertexBuffer already exists");

  glCreateBuffers(1, &id_);
  glBindBuffer(GL_ARRAY_BUFFER, id_);
  glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
}
void VertexBuffer::Dispose() {
  if (*this) {
    glDeleteBuffers(1, &id_);
    id_ = 0;
  }
}
void VertexBuffer::Bind() const { glBindBuffer(GL_ARRAY_BUFFER, id_); }
void VertexBuffer::Unbind() const { glBindBuffer(GL_ARRAY_BUFFER, 0); }
void VertexBuffer::SetData(const void *data, uint size) {
  glBindBuffer(GL_ARRAY_BUFFER, id_);
  glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
}

void IndexBuffer::Setup(uint size) {
  arc_assert(!*this, "IndexBuffer Setup failed! IndexBuffer already exists");
  glCreateBuffers(1, &id_);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
}

void IndexBuffer::Setup(uint *indices, uint count){
  arc_assert(!*this, "IndexBuffer Setup failed! IndexBuffer already exists");
  count_ = count;
  glCreateBuffers(1, &id_);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(indices[0]), indices,
               GL_STATIC_DRAW);
}
void IndexBuffer::Dispose() {
  if (*this) {
    glDeleteBuffers(1, &id_);
    id_ = 0;
  }
}
void IndexBuffer::Bind() const {
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_);
}
void IndexBuffer::Unbind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }
void IndexBuffer::SetData(uint *indices, uint count) {
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_);
  glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, count * sizeof(indices[0]), indices);
}

} // namespace arc
