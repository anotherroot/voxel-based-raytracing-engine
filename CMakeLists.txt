# CMake version
cmake_minimum_required(VERSION 3.17)

include("core/project-meta-info.in")

project(
        arc_game
        VERSION ${project_version}
        DESCRIPTION ${project_description}
        HOMEPAGE_URL ${project_homepage}
        LANGUAGES CXX C
)

set(CMAKE_CXX_STANDARD 17)
set(ARC_PLATFORM_LINUX 1)
set(ARC_PLATFORM_GLFW 1)
set(ARC_PLATFORM_OPENGL 1)

set (CMAKE_EXPORT_COMPILE_COMMANDS ON)
set( CMAKE_POSITION_INDEPENDENT_CODE ON)



SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -g")
SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -g")

# # CCACHE
# find_program(CCACHE_PROGRAM ccache)
# if(CCACHE_PROGRAM)
#     set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CCACHE_PROGRAM}")
# endif()
# # GOLD LINKER
# if (UNIX AND NOT APPLE)
#   execute_process(COMMAND ${CMAKE_C_COMPILER} -fuse-ld=gold -Wl,--version ERROR_QUIET OUTPUT_VARIABLE ld_version)
#   if ("${ld_version}" MATCHES "GNU gold")
#     set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fuse-ld=gold -Wl,--disable-new-dtags")
#     set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -fuse-ld=gold -Wl,--disable-new-dtags")
#   endif()
# endif()




# Only do these if this is the main project
#-------------------------------------
if (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME) #TODO: check waht goes in here and what doesnt
    # C++ standard
    set(CMAKE_CXX_STANDARD 17)

    # Support for folders in IDE's
    set_property(GLOBAL PROPERTY USE_FOLDERS ON)

    # Doxygen
    # Docs only available if this is the main app
    #    find_package(Doxygen)
    #    if(Doxygen_FOUND)
    #        add_subdirectory(docs)
    #    else()
    #        message(STATUS "Doxygen not found, not building docs")
    #    endif()


    # Compiler flags
    #set(CMAKE_CXX_FLAGS "-Wall -Wextra")
#    set(CMAKE_CXX_FLAGS_DEBUG "-g")
#    set(CMAKE_CXX_FLAGS_RELEASE "-O3")

endif ()
#-------------------------------------





add_subdirectory(external)
add_subdirectory(engine)

add_subdirectory(core)

add_executable(desktop)

target_sources(desktop
  PRIVATE
  desktop/main.cpp
  )

target_link_libraries(desktop PUBLIC core)
