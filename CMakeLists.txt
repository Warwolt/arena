cmake_minimum_required(VERSION 3.16.0)
project(arena)
enable_language(C)

# Add Raylib
set(BUILD_SHARED_LIBS ON CACHE BOOL "" FORCE)
add_subdirectory(lib/raylib)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

# Set warnings
add_compile_options(
    /std:clatest # use C23
    /W4     # warning level 4
    /WX     # warnings as errors
    /wd4244 # allow implicit conversion int and float
    /wd4100 # allow function parameters to be declared but not used
    /wd4101 # allow unreferenced local variable
    /wd4189 # allow local variable is initialized but not referenced
    /wd4028 # formal parameter different from declaration (allow forward declare typedef struct)
    /w44062 # warn about missing cases in switch-case
)

# Source
add_library(Source
    src/game.c
    src/core/arena.c
    src/core/sparse_array.c
    src/engine/entity.c
    src/engine/resource.c
    src/game/scene/main_menu.c
    src/game/scene/gameplay.c
    src/platform/logging.c
    src/platform/window.c
    src/raylib_extra.c
)
target_include_directories(Source PUBLIC src lib)
target_link_libraries(Source raylib)

# Main
add_executable(Executable
    src/main.c
)
target_include_directories(Executable PUBLIC src lib)
target_link_libraries(Executable Source)
set_property(TARGET Executable PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Executable)
set_target_properties(Executable PROPERTIES OUTPUT_NAME "Game")

# Tests
add_executable(Tests
    test/main.c
    test/arena_tests.c
    test/entity_tests.c
    test/sparse_array_tests.c
)
target_include_directories(Tests PUBLIC src lib)
target_link_libraries(Tests Source)

# Copy DLL
add_custom_command(TARGET Executable POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:raylib>         # Source DLL
        $<TARGET_FILE_DIR:Executable> # Destination = main exe dir
    COMMENT "Copy raylib DLL to executable directory"
)
