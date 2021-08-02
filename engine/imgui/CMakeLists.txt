cmake_minimum_required(VERSION 3.17)

include("project-meta-info.in")

project(
        ArcImgui
        VERSION ${project_version}
        DESCRIPTION ${project_description}
        HOMEPAGE_URL ${project_homepage}
        LANGUAGES CXX C
)




add_library(arc_imgui OBJECT)
target_sources(arc_imgui
        PUBLIC
        src/imgui_layer.cpp)



# Link libraries to target
target_link_libraries(arc_imgui PUBLIC imgui arc_log)



