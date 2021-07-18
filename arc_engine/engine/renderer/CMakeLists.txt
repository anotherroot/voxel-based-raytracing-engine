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

if(ARC_PLATFORM_OPENGL_ES)
    add_subdirectory(platform/opengl_es)
    target_include_directories(arc_opengl_es PUBLIC ${INCLUDES})
    target_include_directories(arc_renderer PUBLIC platform/opengl_es/include)
    target_link_libraries(arc_renderer PUBLIC arc_opengl_es)
elseif(ARC_PLATFORM_OPENGL)
    add_subdirectory(platform/opengl)
    target_include_directories(arc_opengl PUBLIC ${INCLUDES})
    target_include_directories(arc_renderer PUBLIC platform/opengl/include )
    target_link_libraries(arc_renderer PUBLIC arc_opengl)
else()
    message(FATAL_ERROR "Not supported!")
endif()



target_sources(arc_renderer
        PUBLIC
            src/renderer_api.cpp
            src/buffer.cpp
            #src/font.cpp
            src/graphics_context.cpp
            src/orthographic_camera.cpp
            src/renderer_api.cpp
            src/shader.cpp
            src/batch_renderer.cpp
            src/texture.cpp
            src/vertex_array.cpp
            src/voxel_model.cpp
            src/voxel_renderer.cpp
            src/perspective_camera.cpp
            src/shadow_texture.cpp

        )

target_link_libraries(arc_renderer
        PUBLIC
            arc_log
            glm
            glad
            stb_image
            arc_core
        )


