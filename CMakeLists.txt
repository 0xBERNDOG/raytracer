cmake_minimum_required(VERSION 3.15)
project(raytracer VERSION 0.1 LANGUAGES C)

set(PROJECT_NAME raytracer)
set(TARGET_RELEASE "release")
set(EXEC_NAME_RELEASE "${PROJECT_NAME}")

set(TARGET_UNIT_TESTS "${PROJECT_NAME}_tests")
set(EXEC_NAME_UNIT_TESTS "${PROJECT_NAME}_tests")

# version
configure_file("${CMAKE_SOURCE_DIR}/src/version.h" "${CMAKE_SOURCE_DIR}/src/version.h")

# common
set(SRC_FILES
    "${CMAKE_SOURCE_DIR}/src/engine/image.c"
    "${CMAKE_SOURCE_DIR}/src/engine/lens_ideal.c"
    "${CMAKE_SOURCE_DIR}/src/engine/lens_spherical.c"
    "${CMAKE_SOURCE_DIR}/src/engine/object.c"
    "${CMAKE_SOURCE_DIR}/src/engine/plane.c"
    "${CMAKE_SOURCE_DIR}/src/engine/ray.c"
    "${CMAKE_SOURCE_DIR}/src/engine/sensor.c"
    "${CMAKE_SOURCE_DIR}/src/engine/sphere.c"
    "${CMAKE_SOURCE_DIR}/src/engine/vector.c"
)

set(INCLUDE_DIRS
    "${CMAKE_SOURCE_DIR}/src"
)

set(LINK_LIBRARIES
    "m" # math.h
)

# release
set(RELEASE_SRC_FILES
    "${CMAKE_SOURCE_DIR}/external/libbmp/libbmp.c"
    "${CMAKE_SOURCE_DIR}/src/utils/file.c"
    "${CMAKE_SOURCE_DIR}/src/main.c"
)

set(RELEASE_INCLUDE_DIRS
    "${CMAKE_SOURCE_DIR}/external"
)

message("${TARGET_RELEASE} PUBLIC ${INCLUDE_DIRS} ${RELEASE_INCLUDE_DIRS}")

add_executable(${TARGET_RELEASE} ${SRC_FILES} ${RELEASE_SRC_FILES})
set_target_properties(${TARGET_RELEASE} PROPERTIES OUTPUT_NAME ${EXEC_NAME_RELEASE})
target_compile_options(${TARGET_RELEASE} PUBLIC -g -Wall -Wextra -std=c99 -pedantic-errors -Os)
target_compile_definitions(${TARGET_RELEASE} PUBLIC NDEBUG)
target_include_directories(${TARGET_RELEASE} PUBLIC ${INCLUDE_DIRS} ${RELEASE_INCLUDE_DIRS})
target_link_libraries(${TARGET_RELEASE} ${LINK_LIBRARIES})

# unit tests
set(SRC_FILES_TESTS
    "${CMAKE_SOURCE_DIR}/tests/engine/main.c"
    "${CMAKE_SOURCE_DIR}/tests/engine/lens_ideal.c"
    "${CMAKE_SOURCE_DIR}/tests/engine/lens_spherical.c"
    "${CMAKE_SOURCE_DIR}/tests/engine/plane.c"
    "${CMAKE_SOURCE_DIR}/tests/engine/ray.c"
    "${CMAKE_SOURCE_DIR}/tests/engine/sensor.c"
    "${CMAKE_SOURCE_DIR}/tests/engine/sphere.c"
    "${CMAKE_SOURCE_DIR}/tests/engine/vector.c"
    "${CMAKE_SOURCE_DIR}/tests/main.c"
)

set(INCLUDE_DIRS_TESTS
    "${CMAKE_SOURCE_DIR}"
    "${CMAKE_SOURCE_DIR}/tests"
)

add_executable(${TARGET_UNIT_TESTS} ${SRC_FILES} ${SRC_FILES_TESTS})
set_target_properties(${TARGET_UNIT_TESTS} PROPERTIES OUTPUT_NAME ${EXEC_NAME_UNIT_TESTS})
target_compile_options(${TARGET_UNIT_TESTS} PUBLIC -g3 -Wall -Wextra -std=c99 -pedantic-errors -O0)
target_include_directories(${TARGET_UNIT_TESTS} PUBLIC ${INCLUDE_DIRS} ${INCLUDE_DIRS_TESTS})
target_link_libraries(${TARGET_UNIT_TESTS} ${LINK_LIBRARIES})
add_custom_target(
    "test"
    COMMAND ${TARGET_UNIT_TESTS}
    DEPENDS ${TARGET_UNIT_TESTS}
)
