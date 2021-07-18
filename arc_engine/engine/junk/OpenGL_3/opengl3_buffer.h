//
// Created by adolin on 23. 12. 20.
//

#ifndef ARC_ENGINE_OPENGL3_BUFFER_H
#define ARC_ENGINE_OPENGL3_BUFFER_H


#include "../../renderer/include/buffer.h"

namespace arc{
    class OpenGL3VertexBuffer: public VertexBufferClass{
    public:
        OpenGL3VertexBuffer( uint size);
        OpenGL3VertexBuffer(float *vertices, uint size);
        ~OpenGL3VertexBuffer() override;

        void Bind() const override;

        void Unbind() const override;

        void set_layout(const BufferLayout &layout) override {layout_ = layout;};

        const BufferLayout &layout() const override{return layout_;};

        void SetData(const void *data, uint size) override;

    private:
        BufferLayout layout_{};
        uint renderer_id_;
    };


    class OpenGL3IndexBuffer: public IndexBufferClass{
    public:
        OpenGL3IndexBuffer(uint *indices, uint count);
        OpenGL3IndexBuffer(uint size);
        ~OpenGL3IndexBuffer() override;

        void Bind() const override;

        void Unbind() const override;

        uint count() const override{return count_;}

        void SetData(const void *data, uint size) override;

    private:
        uint count_;
        uint renderer_id_;
    };
}


#endif //ARC_ENGINE_OPENGL3_BUFFER_H
