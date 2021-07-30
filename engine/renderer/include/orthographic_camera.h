//
// Created by adolin on 23. 12. 20.
//

#ifndef ARC_ENGINE_ORTHOGRAPHIC_CAMERA_H
#define ARC_ENGINE_ORTHOGRAPHIC_CAMERA_H

#include <glm.hpp>
namespace arc{
    class OrthographicCamera{
    public:
        void Setup(float screen_ratio,float zoom = 10, float z_near = -100, float z_far = 100,const glm::vec3& position= {0,0,10}, float rotation = 0);


        void SetView(const glm::vec3& position, float rotation = 0);
        void SetProjection(float screen_ratio, float zoom, float z_near = -100, float z_far = 100);

        const glm::mat4& view() const{return view_;}
        const glm::mat4& projection() const{return projection_;}
        const glm::mat4& view_projection() const{return view_projection_;}
    private:
        glm::mat4 view_;
        glm::mat4 projection_;
        glm::mat4 view_projection_;
    };
}



#endif //ARC_ENGINE_ORTHOGRAPHIC_CAMERA_H
