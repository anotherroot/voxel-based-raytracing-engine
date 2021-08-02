cmake_minimum_required(VERSION 3.17)

include("${CMAKE_CURRENT_LIST_DIR}/project-meta-info.in")

project(
        arc_engine
        VERSION ${project_version}
        DESCRIPTION ${project_description}
        HOMEPAGE_URL ${project_homepage}
        LANGUAGES CXX C
)




add_library(glad OBJECT)
target_sources(glad
        PUBLIC
        src/glad.c
        )
target_include_directories(glad PUBLIC include)