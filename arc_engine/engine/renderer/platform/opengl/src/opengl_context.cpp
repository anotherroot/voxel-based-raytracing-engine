//
// Created by adolin on 22. 12. 20.
//

#include <glad/glad.h>
#include "../include/opengl_context.h"
#include "core.h"


arc::OpenGLContext::OpenGLContext(GLFWwindow* window_handle) :window_handle_(window_handle){
    arc_core_assert(window_handle, "Window handle is null!");
}

void arc::OpenGLContext::Init() {
    glfwMakeContextCurrent(window_handle_);

    int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    arc_core_assert(status, "Failed to initialize Glad!");
    logci("OpenGL Info:");
    logci("  Vendor: {0},", glGetString(GL_VENDOR));
    logci("  Renderer: {0},", glGetString(GL_RENDERER));
    logci("  Version: {0},", glGetString(GL_VERSION));

}

void arc::OpenGLContext::SwapBuffers() {
    glfwSwapBuffers(window_handle_);
}
