cmake_minimum_required(VERSION 3.16.0)
project(arena)
enable_language(C)
include(CheckCCompilerFlag)

option(WARNINGS_AS_ERRORS "Compile with warnings as errors" ON)

# Add Raylib
set(CMAKE_C_STANDARD 23)
add_subdirectory(lib/raylib)

# Set warnings
add_compile_options(/W4 /wd4244)
if (WARNINGS_AS_ERRORS)
    add_compile_options(/WX)
endif()

# Source
add_library(source
    src/memory/arena.c
    src/memory/map.c
    src/entity.c
    src/logging.c
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
    test/map_tests.c
)
target_include_directories(tests PUBLIC src lib)
target_link_libraries(tests source)
