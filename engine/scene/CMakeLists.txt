cmake_minimum_required(VERSION 3.17)


add_library(arc_scene STATIC)
target_sources(
        arc_scene
        PUBLIC
        src/scene.cpp
)

target_include_directories(arc_scene PUBLIC include)

target_link_libraries(arc_scene PUBLIC arc_log)
