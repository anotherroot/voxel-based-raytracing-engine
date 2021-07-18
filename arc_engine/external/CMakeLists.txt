cmake_minimum_required(VERSION 3.17)

include("${CMAKE_CURRENT_LIST_DIR}/project-meta-info.in")

project(
        arc_engine
        VERSION ${project_version}
        DESCRIPTION ${project_description}
        HOMEPAGE_URL ${project_homepage}
        LANGUAGES CXX C
)

# Add Window library
if (ARC_PLATFORM_SDL)
    # Build SDL2
    add_subdirectory(SDL2/SDL2)
elseif (ARC_PLATFORM_GLFW)
    # Build glfw
    set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    add_subdirectory(glfw)
    add_compile_definitions(GLFW_INCLUDE_NONE IMGUI_IMPL_OPENGL_LOADER_GLAD)
else()
    message(FATAL_ERROR "Not Supported!")
endif ()

# Build spdlog
add_subdirectory(spdlog)

# Build glm
add_subdirectory(glm)

# Build STB image
add_subdirectory(stb_image)

# Build glad
add_subdirectory(glad)

# Build freetype
add_subdirectory(freetype)

# Build imgui
add_subdirectory(imgui)
target_include_directories(imgui PUBLIC glfw/include glad/include SDL2/SDL2/include)


