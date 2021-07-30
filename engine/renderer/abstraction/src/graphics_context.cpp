//
// Created by adolin on 22. 12. 20.
//

#include "../include/graphics_context.h"
#include "core.h"
#include <glad/glad.h>
#include <glfw3.h>
namespace arc {
void GraphicsContext::Setup(void *window_handle) {
  arc_assert(!(*this), "GraphicsContext Setup failed! GraphicsContext already exists");
  window_handle_ = window_handle;
  arc_core_assert(window_handle, "Window handle is null!");
}
void GraphicsContext::Dispose(){
  if(*this){
    //TODO: look into disposing graphics context
  }
}

void GraphicsContext::Init() {
  glfwMakeContextCurrent((GLFWwindow*)window_handle_);

  int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

  arc_core_assert(status, "Failed to initialize Glad!");
  logci("OpenGL Info:");
  logci("  Vendor: {0},", glGetString(GL_VENDOR));
  logci("  Renderer: {0},", glGetString(GL_RENDERER));
  logci("  Version: {0},", glGetString(GL_VERSION));
}

void GraphicsContext::SwapBuffers() { glfwSwapBuffers((GLFWwindow*)window_handle_); }
} // namespace arc
