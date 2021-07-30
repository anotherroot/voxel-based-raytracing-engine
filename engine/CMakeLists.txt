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
set(INCLUDES log/include ../external/entt/single_include ../external/headers renderer/include core/include imgui/include ../external/glfw/include/GLFW ../external/glad/include ../external/glm/glm)

add_subdirectory(log)
target_include_directories(arc_log PUBLIC ${INCLUDES})

add_subdirectory(core)
target_include_directories(arc_core PUBLIC ${INCLUDES})


add_subdirectory(renderer)
target_include_directories(arc_renderer PUBLIC ${INCLUDES})

add_subdirectory(imgui)
target_include_directories(arc_imgui PUBLIC ${INCLUDES})


add_subdirectory(scene)
target_include_directories(arc_scene PUBLIC ${INCLUDES})



#-------------------------------------


#target_include_directories(arc_renderer PRIVATE extern/glad/include) #TODO: move into renderer
#target_include_directories(arc_renderer PRIVATE extern/SDL2/include)

add_library(engine STATIC)


target_include_directories(engine PUBLIC extern/glad/include INTERFACE include)
#target_include_directories(arc_engine PUBLIC ${SDL_LOCATION}/include)

# Link libraries to target
target_link_libraries(engine PUBLIC arc_renderer arc_core arc_imgui arc_scene dl) #TODO: check GL and dl


target_compile_definitions(engine PUBLIC ARC_PLATFORM_LINUX)
target_compile_definitions(engine PUBLIC ARC_PLATFORM_GLFW)
target_compile_definitions(engine PUBLIC ARC_PLATFORM_OPENGL)



