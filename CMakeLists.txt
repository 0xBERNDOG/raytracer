cmake_minimum_required(VERSION 3.15)
project(raytracer VERSION 0.1 LANGUAGES C)

set(PROJECT_NAME raytracer)
set(TARGET_RELEASE "release")
set(EXEC_NAME_RELEASE "${PROJECT_NAME}")

set(TARGET_UNIT_TESTS "${PROJECT_NAME}_tests")
set(EXEC_NAME_UNIT_TESTS "${PROJECT_NAME}_tests")

# common
set(SRC_FILES
    src/engine/plane.c
    src/engine/vector.c
)

set(INCLUDE_DIRS
    .
    src
    src/engine
    src/utils
)

set(LINK_LIBRARIES
    m # math.h
)

# release
add_executable(${TARGET_RELEASE} ${SRC_FILES} src/main.c)
set_target_properties(${TARGET_RELEASE} PROPERTIES OUTPUT_NAME ${EXEC_NAME_RELEASE})
target_compile_options(${TARGET_RELEASE} PRIVATE -g -Wall -Wextra -std=c99 -pedantic-errors -Os)
target_compile_definitions(${TARGET_RELEASE} PRIVATE NDEBUG)
target_include_directories(${TARGET_RELEASE} PUBLIC ${INCLUDE_DIRS})
target_link_libraries(${TARGET_RELEASE} ${LINK_LIBRARIES})

# unit tests
set(SRC_FILES_TESTS
    tests/engine/main.c
    tests/engine/plane.c
    tests/engine/sphere.c
    tests/engine/vector.c
)

set(INCLUDE_DIRS_TESTS
    tests
    tests/engine
)

add_executable(${TARGET_UNIT_TESTS} ${SRC_FILES} ${SRC_FILES_TESTS} tests/main.c)
set_target_properties(${TARGET_UNIT_TESTS} PROPERTIES OUTPUT_NAME ${EXEC_NAME_UNIT_TESTS})
target_compile_options(${TARGET_UNIT_TESTS} PRIVATE -g3 -Wall -Wextra -std=c99 -pedantic-errors -O0)
target_include_directories(${TARGET_UNIT_TESTS} PUBLIC ${INCLUDE_DIRS} ${INCLUDE_DIRS_TESTS})
target_link_libraries(${TARGET_UNIT_TESTS} ${LINK_LIBRARIES})
add_custom_target(
    "test"
    COMMAND ${TARGET_UNIT_TESTS}
    DEPENDS ${TARGET_UNIT_TESTS}
)
