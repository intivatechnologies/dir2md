cmake_minimum_required(VERSION 3.20)

project(RepoContextClipboardForAi VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(SOURCES
    "main.cpp"
    "misc/get_root_name.hpp"
    "misc/get_content_from_files.hpp"
    "misc/glob_pattern_support.hpp"
    "file_records/filesystem_node.hpp"
    "file_records/filesystem_node.cpp"
    "traversals/traversals_content_roots.hpp"
    "traversals/traversals_string_tree.hpp"
    "test_repurposing/test_util.hpp"
    "test_repurposing/tests.hpp"
    "client/parse_flags.hpp")

# The TREE argument mirrors your disk folders into VS filters
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${SOURCES})

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

add_executable(RepoContextClipboardForAi ${SOURCES})