cmake_minimum_required(VERSION 3.31)
project(DVD_RAYLIB)

set(CMAKE_CXX_STANDARD 20)

add_executable(DVD_RAYLIB src/main.cpp)

add_subdirectory(libs/raylib)
target_link_libraries(DVD_RAYLIB PRIVATE raylib)