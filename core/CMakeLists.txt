cmake_minimum_required(VERSION 3.9)

# Variables
set (ROOT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../)

project(core)

file(GLOB_RECURSE SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

add_library(core STATIC ${SOURCE})

# Includes directory
target_include_directories(core PRIVATE ${ROOT_DIRECTORY}/includes)

# Libraries
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/sqlite3)
target_link_libraries(core PRIVATE sqlite3)
