//
// Created by adolin on 23. 12. 20.
//

#include <glad/glad.h>
#include "opengl3_vertex_array.h"
namespace arc{
    static GLenum ShaderDataTypeToOpenGL3BaseType(ShaderDataType type){
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

    OpenGL3VertexArray::OpenGL3VertexArray() {

        glGenVertexArrays(1, &renderer_id_);
    }

    OpenGL3VertexArray::~OpenGL3VertexArray() {
        glDeleteVertexArrays(1, &renderer_id_);
    }

    void OpenGL3VertexArray::Bind() const {
        glBindVertexArray(renderer_id_);
    }

    void OpenGL3VertexArray::Unbind() const {
        glBindVertexArray(0);
    }

    void OpenGL3VertexArray::BindVertexBuffer(const VertexBuffer& vertex_buffer) {
        glBindVertexArray(renderer_id_);
        vertex_buffer.Bind();

        arc_core_assert(vertex_buffer.layout().elements().size(), "Vertex Buffer has NO Layout!");

        uint index=  0;
        for(const auto& element: vertex_buffer.layout()){
            glEnableVertexAttribArray(index);
            glVertexAttribPointer(index, element.GetComponentCount(),
                                  ShaderDataTypeToOpenGL3BaseType(element.type),
                                  element.normalized? GL_TRUE : GL_FALSE,
                                  vertex_buffer.layout().stride(),
                                  (const void*)element.offset);
            index++;
        }


    }

    void OpenGL3VertexArray::BindIndexBuffer(const IndexBuffer& index_buffer) {
        Bind();
        index_buffer.Bind();

    }
}