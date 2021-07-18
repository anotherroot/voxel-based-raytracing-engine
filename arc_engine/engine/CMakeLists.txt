cmake_minimum_required(VERSION 3.17)

include("project-meta-info.in")

project(
        arc_engine
        VERSION ${project_version}
        DESCRIPTION ${project_description}
        HOMEPAGE_URL ${project_homepage}
        LANGUAGES CXX C
)



# Add Subdirectories
#-------------------------------------
set(INCLUDES log/include ../external/headers renderer/include core/include imgui/include ../external/SDL2/SDL2/include ../external/glfw/include/GLFW ../external/glad/include ../external/glm/glm)

add_subdirectory(log)
target_include_directories(arc_log PUBLIC ${INCLUDES})

add_subdirectory(core)
target_include_directories(arc_core PUBLIC ${INCLUDES})


add_subdirectory(renderer)
target_include_directories(arc_renderer PUBLIC ${INCLUDES})

add_subdirectory(imgui)
target_include_directories(arc_imgui PUBLIC ${INCLUDES})





#-------------------------------------


#target_include_directories(arc_renderer PRIVATE extern/glad/include) #TODO: move into renderer
#target_include_directories(arc_renderer PRIVATE extern/SDL2/include)

add_library(arc_engine STATIC)


target_include_directories(arc_engine PUBLIC extern/glad/include INTERFACE include)
#target_include_directories(arc_engine PUBLIC ${SDL_LOCATION}/include)

# Link libraries to target
target_link_libraries(arc_engine PUBLIC arc_renderer arc_core arc_imgui dl) #TODO: check GL and dl


if(ARC_PLATFORM_ANDROID)
    target_compile_definitions(arc_engine PUBLIC ARC_PLATFORM_ANDROID)
elseif(ARC_PLATFORM_LINUX)
    target_compile_definitions(arc_engine PUBLIC ARC_PLATFORM_LINUX)
else()
    message(FATAL_ERROR "Platform is not supported!")
endif()

if(ARC_PLATFORM_SDL)
    target_compile_definitions(arc_engine PUBLIC ARC_PLATFORM_SDL)
elseif(ARC_PLATFORM_GLFW)
    target_compile_definitions(arc_engine PUBLIC ARC_PLATFORM_GLFW)
else()
    message(FATAL_ERROR "Platform is not supported!")
endif()

if(ARC_PLATFORM_OPENGL_ES)
    target_compile_definitions(arc_engine PUBLIC ARC_PLATFORM_OPENGL_ES)
elseif(ARC_PLATFORM_OPENGL)
    target_compile_definitions(arc_engine PUBLIC ARC_PLATFORM_OPENGL)
else()
    message(FATAL_ERROR "Platform is not supported!")
endif()



