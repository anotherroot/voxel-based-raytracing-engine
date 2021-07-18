//
// Created by adolin on 23. 12. 20.
//

#ifndef ARC_ENGINE_OPENGL_VERTEX_ARRAY_H
#define ARC_ENGINE_OPENGL_VERTEX_ARRAY_H

#include "vertex_array.h"

namespace arc{
    class OpenGLVertexArray : public VertexArrayClass {
    public:
        OpenGLVertexArray();
        virtual ~OpenGLVertexArray();

        void Bind() const override ;

        void Unbind() const override ;

        void BindVertexBuffer(const VertexBuffer& vertex_buffer) override;

        void BindIndexBuffer(const IndexBuffer& index_buffer) override;


    private:
        uint renderer_id_;

    };
}


#endif //ARC_ENGINE_OPENGL_VERTEX_ARRAY_H
