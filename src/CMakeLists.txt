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

find_package(Doxygen)
if(DOXYGEN_FOUND)
    configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in
        ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY
    )
    add_custom_target(doc
        ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
        WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen" VERBATIM
    )
endif(DOXYGEN_FOUND)

install(TARGETS ${target} DESTINATION bin CONFIGURATIONS Release)
