//
// Created by sam on 12/25/20.
//

#include "shader.h"
#ifdef ARC_PLATFORM_OPENGL
#include "opengl_shader.h"
#elif ARC_PLATFORM_OPENGL_ES
#include "opengl_es_shader.h"
#endif

namespace arc{
    void Shader::Create(const std::string &name, const std::string &vertex_source, const std::string &fragment_source) {
        Dispose();
#ifdef ARC_PLATFORM_OPENGL
        shader_ = new OpenGLShader(name, vertex_source, fragment_source);
#elif ARC_PLATFORM_OPENGL_ES
        shader_ = new OpenGLESShader(name, vertex_source, fragment_source);
#else
        arc_core_assert(false, "Not Supported");
#endif

        BindFn();
    }

    void Shader::Create(const std::string &file_path) {
        Dispose();
#ifdef ARC_PLATFORM_OPENGL
        shader_ = new OpenGLShader(file_path);
#elif ARC_PLATFORM_OPENGL_ES
        shader_ = new OpenGLESShader(file_path);
#else
        arc_core_assert(false, "Not Supported");
#endif

        BindFn();
    }

    Shader::~Shader() {

    }
}

