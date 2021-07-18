//
// Created by adolin on 22. 12. 20.
//

#ifndef ARC_ENGINE_WINDOW_H
#define ARC_ENGINE_WINDOW_H

#include "core.h"
#include "graphics_context.h"
#include <string>
namespace arc {

struct WindowProps {
  std::string title;
  int width;
  int height;

  WindowProps(const std::string &title = "Arc Engine", uint width = 1280,
              uint height = 720)
      : title(title), width(width), height(height) {}
};

class Window {
public:
  void Setup(const WindowProps &props);
  void Dispose();
  ~Window() { Dispose(); }
  operator bool() const { return window_; }

  void OnUpdate();
  int width() const{return data_.width;}
  int height() const{return data_.height;}
  void SetVsync(bool enabled);
  void *native_window() const{return window_;}
  void HideCursor();
  void ShowCursor();
  float GetTime();
  void Resize(int width, int height);

private:
  GraphicsContext *context_;
  void *window_{nullptr};
  struct WindowData {
    std::string title;
    uint width, height;
    bool vsync;
  };
  WindowData data_;
};

} // namespace arc

#endif // ARC_ENGINE_WINDOW_H
