cmake_minimum_required(VERSION 3.20)

project(dir2md VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(SOURCES
    "main.cpp"
    "misc/glob_pattern_support.hpp"
    "client/parse_flags.hpp")

# The TREE argument mirrors your disk folders into VS filters
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${SOURCES})

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

add_executable(dir2md ${SOURCES})