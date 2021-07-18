//
// Created by adolin on 23. 12. 20.
//
#include <gtc/matrix_transform.hpp>
#include "orthographic_camera.h"
namespace arc{

    void OrthographicCamera::Setup(float screen_ratio,float zoom, float z_near, float z_far, const glm::vec3 &position, float rotation) {
        SetView(position,rotation);
        SetProjection(screen_ratio,zoom,z_near,z_far);
    }

    void OrthographicCamera::SetView(const glm::vec3 &position, float rotation) {
        view_ = glm::translate(glm::mat4(1), position);
        view_ = glm::inverse(glm::rotate(view_, rotation, {0,0,1}));
        view_projection_ = projection_*view_;
    }

    void OrthographicCamera::SetProjection(float screen_ratio, float zoom, float z_near , float z_far) {
        projection_ = glm::ortho(-zoom*screen_ratio/2.0f,zoom*screen_ratio/2.0f,-zoom/2.0f, zoom/2.0f,-100.0f,100.0f);
        view_projection_ = projection_*view_;
    }
}