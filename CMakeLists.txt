cmake_minimum_required(VERSION 3.16.0)
project(rktest)
enable_language(C)
include(CheckCCompilerFlag)

if(MSVC)
    add_compile_options(/W4 /WX)
else()
    add_compile_options(-Wall -Wextra -Wpedantic -Werror)
endif()

# Source
add_library(source
    src/arena.c
)
target_include_directories(source PUBLIC src)

# Main
add_executable(main
    src/main.c
)
target_include_directories(main PUBLIC src)
target_link_libraries(main source)

# Tests
add_executable(test
    test/main.c
    test/arena_tests.c
)
target_include_directories(test PUBLIC src)
target_link_libraries(test source)
