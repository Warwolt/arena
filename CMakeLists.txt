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
    /wd4244 # disable: implicit conversion int and float
    /wd4100 # disable: function parameters to be declared but not used
    /wd4101 # disable: unreferenced local variable
    /wd4189 # disable: local variable is initialized but not referenced
    /wd4028 # disable: formal parameter different from declaration (allow forward declare typedef struct)
    /wd4127 # disable: conditional expression is constant
    /w44062 # enable: warn about missing cases in switch-case
)

# Source
set(SRC
    src/core/arena.c
    src/core/array_map.c
    src/core/sparse_array.c
    src/engine/entity.c
    src/engine/resource.c
    src/engine/ui.c
    src/game.c
    src/game/scene.c
    src/game/scene/debug.c
    src/game/scene/gameplay.c
    src/game/scene/main_menu.c
    src/platform/assert.c
    src/platform/input.c
    src/platform/logging.c
    src/platform/win32.c
    src/platform/window.c
)
set(LIB
    raylib
    shlwapi
)

add_library(Source STATIC ${SRC})
target_include_directories(Source PUBLIC src lib)
target_link_libraries(Source ${LIB})

add_library(Library SHARED ${SRC})
target_include_directories(Library PUBLIC src lib)
target_link_libraries(Library ${LIB})
set_target_properties(Library PROPERTIES OUTPUT_NAME "Game")

# Main
add_executable(Executable
    src/main.c
    src/platform/win32.c
    src/platform/logging.c
)
target_include_directories(Executable PUBLIC src lib)
target_link_libraries(Executable Library)
set_property(TARGET Executable PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Executable)
set_target_properties(Executable PROPERTIES OUTPUT_NAME "Game")

# Tests
add_executable(Tests
    test/arena_tests.c
    test/array_map_tests.c
    test/entity_tests.c
    test/main.c
    test/sparse_array_tests.c
    test/ui_tests.c
)
target_include_directories(Tests PUBLIC src lib)
target_link_libraries(Tests Source)

# Copy raylib DLL to executable directory
add_custom_command(TARGET Executable POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:raylib>
        $<TARGET_FILE_DIR:Executable>
    COMMENT "Copy raylib DLL to executable directory"
)
