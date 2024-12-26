cmake_minimum_required(VERSION 3.20)

project(main VERSION 1.0)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# set(CMAKE_CXX_FLAGS_DEBUG "-Wall -Wextra -Werror -Wpedantic -Weffc++ -g -D_GLIBCXX_DEBUG")
# set(CMAKE_CXX_FLAGS_RELEASE "-O3")

set(SOURCE_DIR "${CMAKE_SOURCE_DIR}/src")

file(GLOB SRC_FILES "${SOURCE_DIR}/*.cpp" "${SOURCE_DIR}/*.c")

configure_file(${SOURCE_DIR}/config.h.in "${PROJECT_BINARY_DIR}/config.h")

add_executable(main ${SRC_FILES})

find_package(OpenGL REQUIRED)
add_subdirectory("${SOURCE_DIR}/glfw-3.4")

target_link_libraries(main PRIVATE OpenGL::GL glfw)

target_include_directories(main PRIVATE "${SOURCE_DIR}" "${SOURCE_DIR}/glm" PUBLIC "${PROJECT_BINARY_DIR}")

add_custom_target(run
    COMMAND ${CMAKE_COMMAND} --build "${PROJECT_BINARY_DIR}" --clean-first
    COMMAND ./main
)

message(STATUS "Source directory: ${SOURCE_DIR}")
message(STATUS "Binary directory: ${PROJECT_BINARY_DIR}")
message(STATUS "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")