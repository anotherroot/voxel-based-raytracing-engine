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

set (CMAKE_EXPORT_COMPILE_COMMANDS ON)
set( CMAKE_POSITION_INDEPENDENT_CODE ON)


SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -g")
SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -g")

# CCACHE
find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CCACHE_PROGRAM}")
endif()
# GOLD LINKER
if (UNIX AND NOT APPLE)
  execute_process(COMMAND ${CMAKE_C_COMPILER} -fuse-ld=gold -Wl,--version ERROR_QUIET OUTPUT_VARIABLE ld_version)
  if ("${ld_version}" MATCHES "GNU gold")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fuse-ld=gold -Wl,--disable-new-dtags")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -fuse-ld=gold -Wl,--disable-new-dtags")
  endif()
endif()




# Only do these if this is the main project
#-------------------------------------
if (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME) #TODO: check waht goes in here and what doesnt
    # C++ standard
    set(CMAKE_CXX_STANDARD 14)

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

# Submodules
#-------------------------------------

# find_package(GIT QUIET)
# if(GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
#     option(GIT_SUBMODULE "Check submodules during build!" ON)
#     if(GIT_SUBMODULE)
#         message(STATUS "Submodule update!")
#         execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} RESULT_VARIABLE GIT_SUBMOD_RESULT)
#         if(NOT GIT_SUBMOD_RESULT EQUAL "0")
#             message(FATAL_ERROR "git submodule update --init failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
#         endif()
#     endif()
# endif()


#--------------------------------------------

add_subdirectory(arc_engine)


#  Game
#--------------------------------------------

add_subdirectory(core)

if (ANDROID)
    add_subdirectory(android/app/src/main/cpp)
    target_link_libraries(android PRIVATE core)
elseif (UNIX)
    add_executable(desktop)
    target_sources(desktop PRIVATE desktop/main.cpp)
    target_link_libraries(desktop PRIVATE core)
else ()
    message(FATAL_ERROR "Your platform is not supported!")
endif ()

#--------------------------------------------

