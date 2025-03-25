cmake_minimum_required(VERSION 3.11...3.31)

project(
    stl_algorithms_and_ranges
    DESCRIPTION "This is a project description."
    LANGUAGES CXX
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(
    stl_algorithms_and_ranges
    src/stl_algorithms_and_ranges.cpp
    src/functional.cpp
    src/ranges_and_views.cpp
    src/swaps.cpp
    src/compare.cpp
    src/sorting.cpp
    src/partitioning.cpp
    src/divide_and_conquer.cpp
    src/linear_operations.cpp
    src/set_operations.cpp
    src/transformation.cpp
    src/left_folds.cpp
    src/general_reductions.cpp
    src/boolean_reductions.cpp
    src/generators.cpp
    src/copy_and_move.cpp
    src/search_and_compare.cpp
    src/min_max_algorithms.cpp
    src/heap_data_structure.cpp
)

target_include_directories(
    stl_algorithms_and_ranges
    PRIVATE
    ~/.local/include
)

target_compile_options(
    stl_algorithms_and_ranges
    PRIVATE
    -fdiagnostics-color=always
    -Wall
    -Wextra
    -std=c++23
    -Og
    -march=native
)

target_link_directories(
    stl_algorithms_and_ranges
    PRIVATE
    ~/.local/lib
)

target_link_libraries(
    stl_algorithms_and_ranges
    PRIVATE
)

target_link_options(
    stl_algorithms_and_ranges
    PRIVATE
)
