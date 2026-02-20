cmake_minimum_required(VERSION 3.16.0)
project(arena)
enable_language(C)
include(CheckCCompilerFlag)

# Add Raylib
set(CMAKE_C_STANDARD 23)
add_subdirectory(lib/raylib)

# Set warnings
if(MSVC)
    add_compile_options(/W4 /WX /wd4244)
else()
    add_compile_options(-Wall -Wextra -Wpedantic -Werror)
endif()

# Source
add_library(source
    src/arena.c
    src/logging.c
)
target_include_directories(source PUBLIC src)
target_link_libraries(source raylib)

# Main
add_executable(main
    src/main.c
)
target_include_directories(main PUBLIC src)
target_link_libraries(main source)

# Tests
add_executable(tests
    test/main.c
    test/arena_tests.c
)
target_include_directories(tests PUBLIC src)
target_link_libraries(tests source)
