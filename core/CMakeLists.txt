cmake_minimum_required(VERSION 3.17)

include("project-meta-info.in")

project(
        Game
        VERSION ${project_version}
        DESCRIPTION ${project_description}
        HOMEPAGE_URL ${project_homepage}
        LANGUAGES CXX C
)

add_library(core STATIC)
target_sources(core
        PRIVATE
        src/game.cpp
        src/camera_controller.cpp
        src/renderer_system.cpp
        )
target_include_directories(core PUBLIC include)
target_link_libraries(core PUBLIC engine)
