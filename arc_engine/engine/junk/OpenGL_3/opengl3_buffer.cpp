//
// Created by adolin on 23. 12. 20.
//

#include <glad/glad.h>
#include "opengl3_buffer.h"

namespace arc{

    /////////////////////////////////////////////////////////////////////////////
    // Vertex Buffer ////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////


    OpenGL3VertexBuffer::OpenGL3VertexBuffer(uint size) {
        glGenBuffers(1, &renderer_id_);
        glBindBuffer(GL_ARRAY_BUFFER, renderer_id_);
        glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
    }

    OpenGL3VertexBuffer::OpenGL3VertexBuffer(float *vertices, uint size) {
        glGenBuffers(1, &renderer_id_);
        glBindBuffer(GL_ARRAY_BUFFER, renderer_id_);
        glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);

    }

    OpenGL3VertexBuffer::~OpenGL3VertexBuffer() {
        glDeleteBuffers(1, &renderer_id_);
    }

    void OpenGL3VertexBuffer::Bind() const {
        glBindBuffer(GL_ARRAY_BUFFER, renderer_id_);
    }

    void OpenGL3VertexBuffer::Unbind() const {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void OpenGL3VertexBuffer::SetData(const void *data, uint size) {
        glBindBuffer(GL_ARRAY_BUFFER, renderer_id_);
        glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
    }


    /////////////////////////////////////////////////////////////////////////////
    // Index Buffer /////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////


    OpenGL3IndexBuffer::OpenGL3IndexBuffer(uint size) {
        glGenBuffers(1, &renderer_id_);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer_id_);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
    }

    OpenGL3IndexBuffer::OpenGL3IndexBuffer(uint *indices, uint count):count_(count) {
        glGenBuffers(1, &renderer_id_);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer_id_);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, count* sizeof(indices[0]), indices, GL_STATIC_DRAW);
    }


    OpenGL3IndexBuffer::~OpenGL3IndexBuffer() {
        glDeleteBuffers(1, &renderer_id_);
    }

    void OpenGL3IndexBuffer::Bind() const {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer_id_);
    }

    void OpenGL3IndexBuffer::Unbind() const {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    void OpenGL3IndexBuffer::SetData(const void *data, uint size) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer_id_);
        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, size, data);
    }
}
