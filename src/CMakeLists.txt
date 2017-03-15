cmake_minimum_required(VERSION 3.0)

project(NVS VERSION 1.0.0 LANGUAGES C)

if ("${CMAKE_C_COMPILER_ID}" STREQUAL "AppleClang" OR
    "${CMAKE_C_COMPILER_ID}" STREQUAL "Clang" OR
    "${CMAKE_C_COMPILER_ID}" STREQUAL "GNU")
    add_compile_options(-std=c99 -Wall -Wextra -Werror -pedantic)
endif()

configure_file("${PROJECT_SOURCE_DIR}/main.h.in" "${PROJECT_BINARY_DIR}/main.h")
include_directories("${PROJECT_BINARY_DIR}")

string(TOLOWER ${PROJECT_NAME} target)
file(GLOB sources *.c *.h *.in)
add_executable(${target} ${sources})

find_package(ZLIB)
if (ZLIB_FOUND)
    include_directories(${ZLIB_INCLUDE_DIRS})
    target_link_libraries(${target} ${ZLIB_LIBRARIES})
endif()

install(TARGETS ${target} DESTINATION bin CONFIGURATIONS Release)
