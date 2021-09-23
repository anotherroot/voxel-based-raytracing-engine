cmake_minimum_required(VERSION 3.17)

include("${CMAKE_CURRENT_LIST_DIR}/project-meta-info.in")



project(
        ArcCore
        VERSION ${project_version}
        DESCRIPTION ${project_description}
        HOMEPAGE_URL ${project_homepage}
        LANGUAGES CXX C
)
add_library(arc_renderer STATIC)

add_subdirectory(abstraction)
target_include_directories(arc_render_abst PUBLIC ${INCLUDES})
target_include_directories(arc_renderer PUBLIC abstraction/include )
target_link_libraries(arc_renderer PUBLIC arc_render_abst)



target_sources(arc_renderer
        PUBLIC
            src/orthographic_camera.cpp
            src/batch_renderer.cpp
            src/voxel_model.cpp
            src/voxel_renderer.cpp
            src/perspective_camera.cpp
            src/overlay_renderer.cpp

        )

target_link_libraries(arc_renderer
        PUBLIC
            arc_log
            glm
            glad
            stb_image
            arc_core
            arc_scene
        )


