cmake_minimum_required(VERSION 3.17)

include("${CMAKE_CURRENT_LIST_DIR}/project-meta-info.in")



project(
        ArcCore
        VERSION ${project_version}
        DESCRIPTION ${project_description}
        HOMEPAGE_URL ${project_homepage}
        LANGUAGES CXX C
)

add_library(arc_core OBJECT include/functions.h)
target_sources(arc_core
        PUBLIC
            src/engine.cpp
            src/event_handler.cpp
            src/window.cpp
            src/input.cpp
        )

target_include_directories(arc_core PUBLIC ../renderer/abstraction/include )

target_link_libraries(arc_core PUBLIC arc_log)






