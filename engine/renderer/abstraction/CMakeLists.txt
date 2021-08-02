cmake_minimum_required(VERSION 3.17)

include("project-meta-info.in")



project(
        ArcLinuxSDL
        VERSION ${project_version}
        DESCRIPTION ${project_description}
        HOMEPAGE_URL ${project_homepage}
        LANGUAGES CXX C
)

add_library(arc_render_abst OBJECT )

target_link_libraries(arc_render_abst PUBLIC glfw)


target_sources(arc_render_abst
        PUBLIC
            src/graphics_context.cpp
            src/texture.cpp
            src/renderer_api.cpp
            src/shader.cpp
            src/buffer.cpp
            src/vertex_array.cpp
            src/frame_buffer.cpp
        )

target_include_directories(arc_render_abst PUBLIC include)

target_link_libraries(arc_render_abst PUBLIC arc_log freetype)
