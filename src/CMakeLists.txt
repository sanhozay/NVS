cmake_minimum_required(VERSION 3.0)

configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/main.h.in"
    "${PROJECT_BINARY_DIR}/main.h"
)
include_directories("${PROJECT_BINARY_DIR}")

set(target nvs)
file(GLOB sources *.c *.h *.in)
add_executable(${target} ${sources})

find_package(ZLIB)
if (ZLIB_FOUND)
    include_directories(${ZLIB_INCLUDE_DIRS})
    target_link_libraries(${target} ${ZLIB_LIBRARIES})
endif()

install(TARGETS ${target} DESTINATION bin CONFIGURATIONS Release)
