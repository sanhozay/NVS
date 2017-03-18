cmake_minimum_required(VERSION 3.0)

project(NVS VERSION 1.1.0 LANGUAGES C)

if ("${CMAKE_C_COMPILER_ID}" STREQUAL "AppleClang" OR
    "${CMAKE_C_COMPILER_ID}" STREQUAL "Clang" OR
    "${CMAKE_C_COMPILER_ID}" STREQUAL "GNU")
    add_compile_options(-std=c99 -Wall -Wextra -Werror -pedantic)
endif()

add_subdirectory(src)
