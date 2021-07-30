#ifndef ARC_ENGINE_PERSPECTIVE_CAMERA_H
#define ARC_ENGINE_PERSPECTIVE_CAMERA_H

#include <glm.hpp>
namespace arc {
class PerspectiveCamera {
public:
  void Setup(const glm::vec3 &pos, const glm::vec3 &look_at,
             const glm::vec3 &up, float screen_ratio, float fov, float z_near,
             float z_far);

  void SetView(const glm::vec3 &position, const glm::vec3 &look_at,
               const glm::vec3 &up_vector = {0, 1, 0});
  void SetProjection(float screen_ratio, float fov, float z_near = 0.1f,
                     float z_far = 100);

  const glm::mat4 &view() const { return view_; }
  const glm::mat4 &projection() const { return projection_; }
  const glm::mat4 &view_projection() const { return view_projection_; }

private:
  glm::mat4 view_;
  glm::mat4 projection_;
  glm::mat4 view_projection_;
};
} // namespace arc

#endif // ARC_ENGINE_PERSPECTIVE_CAMERA_H
