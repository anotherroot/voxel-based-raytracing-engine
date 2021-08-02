#include "perspective_camera.h"
#include <gtc/matrix_transform.hpp>
namespace arc {

void PerspectiveCamera::Setup(const glm::vec3 &pos, const glm::vec3 &look_at,
                              const glm::vec3 &up, float screen_ratio,
                              float fov, float z_near, float z_far) {

  SetView(pos, look_at, up);
  SetProjection(screen_ratio, fov, z_near, z_far);
}

void PerspectiveCamera::SetView(const glm::vec3 &position,
                                const glm::vec3 &look_at,
                                const glm::vec3 &up_vector) {
  view_ = glm::lookAt(position, position + look_at, up_vector);
  view_projection_ = projection_ * view_;
}

void PerspectiveCamera::SetProjection(float screen_ratio, float fov,
                                      float z_near, float z_far) {
  projection_ =
      glm::perspective(glm::radians(fov), screen_ratio, z_near, z_far);
  view_projection_ = projection_ * view_;
}
} // namespace arc
