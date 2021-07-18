cmake_minimum_required(VERSION 3.17)

include("${CMAKE_CURRENT_LIST_DIR}/project-meta-info.in")

project(
        arc_engine
        VERSION ${project_version}
        DESCRIPTION ${project_description}
        HOMEPAGE_URL ${project_homepage}
        LANGUAGES CXX C
)




add_library(stb_image OBJECT)
target_sources(stb_image PUBLIC src/image.cpp)

if (ARC_PLATFORM_SDL)
    target_link_libraries(stb_image PRIVATE SDL2)
endif()

target_include_directories(stb_image PUBLIC include)

