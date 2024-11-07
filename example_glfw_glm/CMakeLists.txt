cmake_minimum_required(VERSION 3.20)

project(main VERSION 1.0)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB SRC_FILES "*.cpp" "*.c")
include_directories(${PROJECT_SOURCE_DIR})
add_executable(main ${SRC_FILES})

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

target_link_libraries(main PRIVATE OpenGL::GL glfw)
