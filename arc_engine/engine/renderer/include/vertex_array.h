//
// Created by adolin on 23. 12. 20.
//

#ifndef ARC_ENGINE_VERTEX_ARRAY_H
#define ARC_ENGINE_VERTEX_ARRAY_H

#include <vector>
#include "buffer.h"

namespace arc{
    class VertexArrayClass {
    public:
        virtual ~VertexArrayClass() {}

        virtual void Bind() const = 0;

        virtual void Unbind() const = 0;

        virtual void BindVertexBuffer(const VertexBuffer& vertex_buffer) = 0;

        virtual void BindIndexBuffer(const IndexBuffer& index_buffer) = 0;



    };

    class VertexArray: public Caller{
    private:

        typedef void(VoidMemberFn)(void*);
        typedef void(VoidMemberVBFn)(void*,const VertexBuffer& );
        typedef void(VoidMemberIBFn)(void*,const IndexBuffer& );


        VertexArrayClass* vertex_array_{nullptr};

        VoidMemberFn* bind_fn_, *unbind_fn_;
        VoidMemberVBFn *bind_vertex_buffer_fn_;
        VoidMemberIBFn *bind_index_buffer_fn_;
    public:
        ~VertexArray(){
            Dispose();
        }
        inline void Dispose(){
            if(vertex_array_!= nullptr){
                delete vertex_array_;
                vertex_array_ = nullptr;
            }
        }
        void Create();
        inline void Bind() const {
            bind_fn_(vertex_array_);
        }

        inline void Unbind() const {
            unbind_fn_(vertex_array_);
        }

        inline void AddVertexBuffer(const VertexBuffer& vertex_buffer) {
            bind_vertex_buffer_fn_(vertex_array_, vertex_buffer);
        }

        inline void set_index_buffer(const IndexBuffer& index_buffer) {
            bind_index_buffer_fn_(vertex_array_, index_buffer);
        }




    };
}


#endif //ARC_ENGINE_VERTEX_ARRAY_H
