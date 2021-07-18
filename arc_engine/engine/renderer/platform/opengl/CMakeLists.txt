cmake_minimum_required(VERSION 3.17)

include("project-meta-info.in")



project(
        ArcLinuxSDL
        VERSION ${project_version}
        DESCRIPTION ${project_description}
        HOMEPAGE_URL ${project_homepage}
        LANGUAGES CXX C
)

add_library(arc_opengl OBJECT )

if (ARC_PLATFORM_GLFW)
    target_sources(arc_opengl PUBLIC src/opengl_context.cpp)
    target_link_libraries(arc_opengl PUBLIC glfw)
elseif(ARC_PLATFORM_SDL)
    target_sources(arc_opengl PUBLIC src/opengl_context_sdl.cpp)
    target_link_libraries(arc_opengl PUBLIC SDL2)
else()
    message(FATAL_ERROR "Not supported!")
endif ()


target_sources(arc_opengl
        PUBLIC
            src/opengl_texture.cpp
            src/opengl_renderer_api.cpp
            src/opengl_shader.cpp
            src/opengl_vertex_array.cpp
            src/opengl_font.cpp
            src/frame_buffer.cpp
        )

target_include_directories(arc_opengl PUBLIC include)

target_link_libraries(arc_opengl PUBLIC arc_log freetype)
