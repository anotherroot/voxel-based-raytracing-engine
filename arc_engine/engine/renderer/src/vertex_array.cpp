//
// Created by adolin on 23. 12. 20.
//

#include "vertex_array.h"


#ifdef ARC_PLATFORM_OPENGL
#include "opengl_vertex_array.h"
#elif ARC_PLATFORM_OPENGL_ES
#include "opengl_es_vertex_array.h"
#endif
namespace arc{


    void VertexArray::Create() {
        Dispose();
#ifdef ARC_PLATFORM_OPENGL
        vertex_array_ = new OpenGLVertexArray();
#elif ARC_PLATFORM_OPENGL_ES
        vertex_array_ = new OpenGLESVertexArray();
#else
        arc_core_assert(false, "Not Supported");
#endif
        bind_fn_ = (VoidMemberFn*)GetVTable(vertex_array_)[2];
        unbind_fn_ = (VoidMemberFn*)GetVTable(vertex_array_)[3];
        bind_vertex_buffer_fn_ = (VoidMemberVBFn *)GetVTable(vertex_array_)[4];
        bind_index_buffer_fn_ = (VoidMemberIBFn *)GetVTable(vertex_array_)[5];
    }
}