# CMake version
cmake_minimum_required(VERSION 3.17)

include("project-meta-info.in")

project(
        arc_engine
        VERSION ${project_version}
        DESCRIPTION ${project_description}
        HOMEPAGE_URL ${project_homepage}
        LANGUAGES CXX C
)


# Platform variables
#-------------------------------------
if (ANDROID)
    set(ARC_PLATFORM_ANDROID 1)
    set(ARC_PLATFORM_SDL 1)
    set(ARC_PLATFORM_OPENGL_ES 1)
elseif (UNIX)
    set(ARC_PLATFORM_LINUX 1)
    set(ARC_PLATFORM_GLFW 1)
    #set(ARC_PLATFORM_SDL 1)
    set(ARC_PLATFORM_OPENGL 1)
else ()
    message(FATAL_ERROR "Your platform is not supported!")
endif ()
#-------------------------------------





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

find_package(GIT QUIET)
if(GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
    option(GIT_SUBMODULE "Check submodules during build!" ON)
    if(GIT_SUBMODULE)
        message(STATUS "Submodule update!")
        execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} RESULT_VARIABLE GIT_SUBMOD_RESULT)
        if(NOT GIT_SUBMOD_RESULT EQUAL "0")
            message(FATAL_ERROR "git submodule update --init failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
        endif()
    endif()
endif()


#--------------------------------------------

add_subdirectory(external)
add_subdirectory(engine)





#

