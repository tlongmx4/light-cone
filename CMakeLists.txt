cmake_minimum_required(VERSION 4.3)
project(light_cone)

set(CMAKE_CXX_STANDARD 20)

find_package(raylib REQUIRED)

add_executable(light_cone main.cpp)

target_link_libraries(light_cone PRIVATE raylib)
