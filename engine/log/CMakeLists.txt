cmake_minimum_required(VERSION 3.17)


add_library(arc_log STATIC)
target_sources(
        arc_log
        PUBLIC
        src/log.cpp
)


target_link_libraries(arc_log PUBLIC spdlog)