#include "camera_controller.h"
#include "buttons.h"
#include "engine.h"
#include "input.h"
#include "keys.h"
CameraController::CameraController()
    : fov_(60), look_at_({0, 0, -1}), position_({0, 0, 10}), pitch_(0),
      yaw_(180), right_({1, 0, 0}), fp_controller_(false) {
  arc::EventHandler::SubscribeKeyboard(this);
  arc::EventHandler::SubscribeMouse(this);
  arc::EventHandler::SubscribeWindow(this);
  float ratio = (float)arc::Engine::window().width() /
                (float)arc::Engine::window().height();
  camera_.Setup(position_, look_at_, {0, 1, 0}, ratio, fov_, 0.1, 100);
}
void CameraController::Update(float delta_time) {
  position_ += direction_ * delta_time * 5.0f;
  camera_.SetView(position_, look_at_);
  moved_ |= glm::length(direction_)>0.1;
}
bool CameraController::OnKeyDown(int key, bool repeat) {
  CheckKeys();
  if (!repeat && key == arc::Keys::C) {
    fp_controller_ = !fp_controller_;
    if (fp_controller_) {
      arc::Engine::window().HideCursor();
      prev_mouse_position_ = {arc::Input::GetMouseX(), arc::Input::GetMouseY()};
    } else {
      arc::Engine::window().ShowCursor();
    }
  }
  return false;
}
bool CameraController::OnKeyUp(int key) {
  CheckKeys();
  return false;
}
void CameraController::CheckKeys() {
  direction_ = {0, 0, 0};
  if (arc::Input::IsKeyPressed(arc::Keys::W)) {
    direction_ += look_at_;
  }
  if (arc::Input::IsKeyPressed(arc::Keys::S)) {
    direction_ -= look_at_;
  }
  if (arc::Input::IsKeyPressed(arc::Keys::D)) {
    direction_ += right_;
  }
  if (arc::Input::IsKeyPressed(arc::Keys::A)) {
    direction_ -= right_;
  }
  if (arc::Input::IsKeyPressed(arc::Keys::SPACE)) {
    direction_.y += 1;
  }
  if (arc::Input::IsKeyPressed(arc::Keys::LEFT_SHIFT)) {
    direction_.y -= 1;
  }
}
bool CameraController::OnWindowResize(int width, int height) {
  float ratio = (float)width / (float)height;
  camera_.SetProjection(ratio, fov_);
  return false;
}
bool CameraController::OnButtonDown(int button) { return false; }
bool CameraController::OnButtonUp(int button) { return false; }
bool CameraController::OnScroll(double x_offset, double y_offset) {
  return false;
}
bool CameraController::OnMove(double x_pos, double y_pos) {
  if (!fp_controller_)
    return false;
  glm::vec2 mouse_position = glm::vec2(x_pos, y_pos);
  glm::vec2 dir = (prev_mouse_position_ - mouse_position) * MOUSE_MOVE_DELTA;
  yaw_ += dir.x;
  pitch_ = glm::clamp(pitch_ + dir.y, -89.0f, 89.0f);
  if (yaw_ < 0) {
    yaw_ += 360;
  } else if (yaw_ >= 360) {
    yaw_ -= 360;
  }
  look_at_ =
      glm::vec3(glm::sin(glm::radians(yaw_)) * glm::cos(glm::radians(pitch_)),
                glm::sin(glm::radians(pitch_)),
                glm::cos(glm::radians(yaw_)) * glm::cos(glm::radians(pitch_)));
  right_ = glm::normalize(glm::cross(look_at_, up_vector_));
  camera_.SetView(position_, look_at_, {0, 1, 0});
  prev_mouse_position_ = mouse_position;
  moved_ = true;
  return false;
}

void CameraController::Rotate(float yaw, float pitch) {}
