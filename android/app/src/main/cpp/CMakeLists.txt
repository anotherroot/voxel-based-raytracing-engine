cmake_minimum_required(VERSION 3.17)

add_library(test_android SHARED)

target_sources(test_android PRIVATE main.cpp)

#target_link_libraries(test_android PUBLIC SDL2)
