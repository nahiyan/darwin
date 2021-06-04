cmake_minimum_required(VERSION 3.9)

# Variables
set(ROOT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../../)

project(core)

file(GLOB_RECURSE SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

add_library(core SHARED ${SOURCE})

# Includes directory
target_include_directories(
  core
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/public/core
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/private)
