cmake_minimum_required(VERSION 3.20)

project(main)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories("${CMAKE_SOURCE_DIR}")
add_executable(main "main.cpp" "glad.c")

find_package(glfw3 REQUIRED)

target_link_libraries(main glfw)