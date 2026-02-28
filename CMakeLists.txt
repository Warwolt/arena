cmake_minimum_required(VERSION 3.16.0)
project(arena)
enable_language(C)
include(CheckCCompilerFlag)

option(WARNINGS_AS_ERRORS "Compile with warnings as errors" ON)

# Add Raylib
set(CMAKE_C_STANDARD 23)
add_subdirectory(lib/raylib)

# Set warnings
add_compile_options(
    /W4
    /wd4244 # allow implicit conversion int and float
    /wd4100 # allow function parameters to be declared but not used
    /wd4028 # formal parameter different from declaration (allow forward declare typedef struct)
    /w44062 # warn about missing cases in switch-case
)
if (WARNINGS_AS_ERRORS)
    add_compile_options(/WX)
endif()

# Source
add_library(source
    src/arena.c
    src/entity.c
    src/logging.c
    src/raylib_extra.c
    src/resource.c
    src/scene.c
    src/sparse_array.c
    src/win32.c
)
target_include_directories(source PUBLIC src lib)
target_link_libraries(source raylib)

# Main
add_executable(main
    src/main.c
)
target_include_directories(main PUBLIC src lib)
target_link_libraries(main source)
set_property(TARGET main PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT main)

# Tests
add_executable(tests
    test/main.c
    test/arena_tests.c
    test/entity_tests.c
    test/sparse_array_tests.c
)
target_include_directories(tests PUBLIC src lib)
target_link_libraries(tests source)
