cmake_minimum_required(VERSION 3.31)
project(DVD_SDL3)

set(CMAKE_CXX_STANDARD 20)

add_executable(DVD_SDL3 src/main.cpp)

add_subdirectory(libs/raylib)
target_link_libraries(DVD_SDL3 PRIVATE raylib)

file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})