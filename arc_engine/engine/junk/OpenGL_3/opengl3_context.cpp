//
// Created by adolin on 22. 12. 20.
//


#include "opengl3_context.h"
#include "../../core/include/core.h"
#include <glad/glad.h>

arc::OpenGL3Context::OpenGL3Context(GLFWwindow* window_handle) :window_handle_(window_handle){
    arc_core_assert(window_handle, "Window handle is null!");
}

void arc::OpenGL3Context::Init() {
    glfwMakeContextCurrent(window_handle_);

    int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    arc_core_assert(status, "Failed to initialize Glad!");
    logci("OpenGL3 Info:");
    logci("  Vendor: {0},", glGetString(GL_VENDOR));
    logci("  Renderer: {0},", glGetString(GL_RENDERER));
    logci("  Version: {0},", glGetString(GL_VERSION));
}

void arc::OpenGL3Context::SwapBuffers() {
    glfwSwapBuffers(window_handle_);
}


