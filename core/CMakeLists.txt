cmake_minimum_required(VERSION 3.9)

# Variables
set (CMAKE_CXX_STANDARD 17)
set (ROOT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../)

project(core)

add_library(core STATIC
    src/Filesystem.cpp
)

# Includes directory
target_include_directories(core PRIVATE ${ROOT_DIRECTORY}/includes)

# Libraries
# target_link_libraries(core PRIVATE sqlite3)
# target_link_directories(core PRIVATE ${ROOT_DIRECTORY}/libraries)
