cmake_minimum_required(VERSION 3.16.0)
project(arena)
enable_language(C)
include(CheckCCompilerFlag)

# Add Raylib
add_subdirectory(lib/raylib)

# Set warnings
add_compile_options(
    /std:clatest # use C23
    /W4
    /WX
    /wd4244 # allow implicit conversion int and float
    /wd4100 # allow function parameters to be declared but not used
    /wd4101 # allow unreferenced local variable
    /wd4189 # allow local variable is initialized but not referenced
    /wd4028 # formal parameter different from declaration (allow forward declare typedef struct)
    /w44062 # warn about missing cases in switch-case
)

# Source
add_library(source
    src/core/arena.c
    src/core/sparse_array.c
    src/engine/entity.c
    src/engine/resource.c
    src/game/game.c
    src/game/scene/main_menu.c
    src/game/scene/gameplay.c
    src/platform/logging.c
    src/platform/window.c
    src/raylib_extra.c
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
