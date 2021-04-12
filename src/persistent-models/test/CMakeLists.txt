cmake_minimum_required(VERSION 3.18)

set(CMAKE_CXX_STANDARD 11)
project(pm-test)
set(BUNDLE OFF)
set(SOURCES main.cpp)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../ pm)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../lib/googletest googletest)

add_executable(pm-test ${SOURCES})
target_include_directories(
  pm-test
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../lib/googletest/googletest/
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../include)
target_link_libraries(pm-test pm gtest)
