#ifndef CORE_CAMERA_CONTROLLER_H
#define CORE_CAMERA_CONTROLLER_H
#include "listeners.h"
#include "perspective_camera.h"
class CameraController : public arc::KeyboardListener,
                         arc::MouseListener,
                         arc::WindowListener {
  const float MOUSE_MOVE_DELTA = 0.25f;
public:
  CameraController();
  void Update(float delta_time);
  const arc::PerspectiveCamera &camera() const { return camera_; }
  bool CheckIfMoved(){
    if(moved_){
      moved_=false;
      return true;
    }
    return false;
  } 
private:
  float speed_{5.0};
  bool moved_{false};
  bool fp_controller_;
  float fov_, yaw_, pitch_;
  glm::vec3 position_;
  glm::vec2 rotate_;
  glm::vec3 direction_;
  glm::vec3 look_at_, right_;
  glm::vec3 up_vector_{0, 1, 0};

  glm::vec2 prev_mouse_position_;

  bool OnKeyDown(int key, bool repeat) override;
  bool OnKeyUp(int key) override;
  bool OnWindowResize(int width, int height) override;
  bool OnButtonDown(int button) override;
  bool OnButtonUp(int button) override;
  bool OnScroll(double x_offset, double y_offset) override;
  bool OnMove(double x_pos, double y_pos) override;

  void CheckKeys();
  void Rotate(float yaw, float pitch);
  arc::PerspectiveCamera camera_;
};
#endif
