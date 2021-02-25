cmake_minimum_required(VERSION 3.13)
project(GLFWStarter C CXX)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
endif()
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

set(CMAKE_CXX_STANDARD 17)

include(cmake/shared_settings.cmake)

add_subdirectory(extern)
add_subdirectory(glfw_starter)
