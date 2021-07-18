//
// Created by adolin on 21. 12. 20.
//

#include "opengl3_renderer_api.h"
#include "include/includes.h"
#include <glad/glad.h>
namespace arc{
    void OpenGL3RendererAPI::Init() {
        logi("Opengl renderer api init!");
    }

    void OpenGL3RendererAPI::SetViewport(uint x, uint y, uint width, uint height) {
        glViewport(x,y,width,height);
    }

    void OpenGL3RendererAPI::SetClearColor(const glm::vec4 &color) {
        glClearColor(color.r, color.g, color.b, color.a);
    }

    void OpenGL3RendererAPI::Clear() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void OpenGL3RendererAPI::DrawIndexed(const VertexArray& vertex_array, uint count) {
        glDrawElements(GL_TRIANGLES, count,GL_UNSIGNED_INT, nullptr);
    }

}

