cmake_minimum_required(VERSION 3.30.5)
project(DVD_SDL3)

set(CMAKE_CXX_STANDARD 20)

add_executable(DVD_SDL3 main.cpp)

add_subdirectory(libs/sdl)
add_subdirectory(libs/sdl_image)
add_subdirectory(libs/sdl_mixer)

target_link_libraries(DVD_SDL3 SDL3::SDL3)
target_link_libraries(DVD_SDL3 SDL3_image::SDL3_image)
target_link_libraries(DVD_SDL3 SDL3_mixer::SDL3_mixer)

file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})