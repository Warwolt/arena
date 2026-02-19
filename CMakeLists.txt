cmake_minimum_required(VERSION 3.16.0)
project(rktest)
enable_language(C)
include(CheckCCompilerFlag)

if(MSVC)
    add_compile_options(/W4 /WX)
else()
    add_compile_options(-Wall -Wextra -Wpedantic -Werror)
endif()

# Main
add_executable(main
    src/main.c
)

# Source
# todo add source code static lib

# Tests
add_executable(tests
    test/main.c
    test/arena_tests.c
)
target_include_directories(tests PUBLIC src)
