cmake_minimum_required(VERSION 3.9)

# Variables
set (ROOT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../)

project(core)

add_library(core STATIC
    src/Database.cpp
)

# Includes directory
target_include_directories(core PRIVATE ${ROOT_DIRECTORY}/includes)

# Libraries
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/sqlite3)
target_link_libraries(core PRIVATE sqlite3)
