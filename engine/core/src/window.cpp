//
// Created by adolin on 22. 12. 20.
//

#include <glfw3.h>
#include "window.h"
#include "core.h"
#include "event_handler.h"
namespace arc {
static bool s_glfw_initialized = false;
static void GLFWErrorCallback(int error, const char *message) {
  logce("GLFW Error ({0}): {1}", error, message);
}
void Window::Setup(const WindowProps &props) {

  arc_assert(!*this, "Window Setup failed! Window already exists");

  data_.title = props.title;
  data_.width = props.width;
  data_.height = props.height;

  logci("Creating window {0} ({1}, {2})", props.title, props.width,
        props.height);

  if (!s_glfw_initialized) {
    // TODO: glfwterminate on system shutdown
    int success = glfwInit();
    arc_core_assert(success, "Could not initialize GLFW!");
    glfwSetErrorCallback(GLFWErrorCallback);

    s_glfw_initialized = true;
  }
  window_ = glfwCreateWindow(props.width, props.height, props.title.c_str(),
                             nullptr, nullptr);

  context_.Setup(window_);
  context_.Init();

  SetVsync(true);

  glfwSetWindowSizeCallback((GLFWwindow*)window_,
                            [](GLFWwindow *window, int width, int height) {
                              EventHandler::OnResize(width, height);
                            });

  glfwSetWindowCloseCallback(
      (GLFWwindow*)window_, [](GLFWwindow *window) { EventHandler::OnClose(); });
  glfwSetCharCallback((GLFWwindow*)window_, [](GLFWwindow *window, uint key_code) {
    EventHandler::OnChar(key_code);
  });

  glfwSetKeyCallback((GLFWwindow*)window_, [](GLFWwindow *window, int key, int scan_code,
                                 int action, int mods) {
    switch (action) {
    case GLFW_PRESS:
      EventHandler::OnKeyDown(key, false);
      break;
    case GLFW_RELEASE:
      EventHandler::OnKeyUp(key);
      break;
    case GLFW_REPEAT:
      EventHandler::OnKeyDown(key, true);
      break;
    }
  });

  glfwSetMouseButtonCallback(
      (GLFWwindow*)window_, [](GLFWwindow *window, int button, int action, int mods) {
        switch (action) {
        case GLFW_PRESS: {
          EventHandler::OnButtonDown(button);
          break;
        }
        case GLFW_RELEASE: {
          EventHandler::OnButtonUp(button);
          break;
        }
        }
      });

  glfwSetScrollCallback(
      (GLFWwindow*)window_, [](GLFWwindow *window, double x_offset, double y_offset) {
        EventHandler::OnScroll(x_offset, y_offset);
      });

  glfwSetCursorPosCallback((GLFWwindow*)window_,
                           [](GLFWwindow *window, double x_pos, double y_pos) {
                             EventHandler::OnMove(x_pos, y_pos);
                           });
}
void Window::Dispose() {
  if (*this) {
    glfwDestroyWindow((GLFWwindow*)window_);
    window_ = nullptr;
  }
}
void Window::OnUpdate() {
  glfwPollEvents();
  context_.SwapBuffers();
}

void Window::SetVsync(bool enable) {
  if (enable) {
    glfwSwapInterval(1);
  } else {
    glfwSwapInterval(0);
  }
  data_.vsync = enable;
}
void Window::HideCursor() {
  glfwSetInputMode((GLFWwindow*)window_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}
void Window::ShowCursor() {
  glfwSetInputMode((GLFWwindow*)window_, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}
float Window::GetTime() { return (float)glfwGetTime(); }
void Window::Resize(int width, int height) {
  data_.width = width;
  data_.height = height;
}
void Window::SetSize(int width, int height){
  glfwSetWindowSize((GLFWwindow*)window_,width,height);
  /* glfwGetWindowSize((GLFWwindow*)window_,&width,&height); */
  /* data_.width = width; */
  /* data_.height = height; */
}
} // namespace arc
