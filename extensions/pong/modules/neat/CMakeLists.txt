cmake_minimum_required(VERSION 3.20)
set(CMAKE_CXX_STANDARD 11)
project(pong-neat)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

file(GLOB SRC_FILES ${PROJECT_SOURCE_DIR}/src/*.cpp)
add_library(neat SHARED ${SRC_FILES})

target_include_directories(
  neat
  PRIVATE ${PROJECT_SOURCE_DIR}/../../godot-cpp/godot-headers
          ${PROJECT_SOURCE_DIR}/../../godot-cpp/include/core
          ${PROJECT_SOURCE_DIR}/../../godot-cpp/include/gen
          ${PROJECT_SOURCE_DIR}/../../godot-cpp/include
          ${PROJECT_SOURCE_DIR}/include)
target_link_directories(neat PRIVATE ${PROJECT_SOURCE_DIR}/../../godot-cpp/bin)
target_link_libraries(neat godotcpp)

# file(COPY $<TARGET_FILE:neat> DESTINATION
# ${PROJECT_SOURCE_DIR}/../../bin/$<TARGET_FILE_NAME:neat>)

add_custom_command(
  TARGET neat
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:neat>
          ${PROJECT_SOURCE_DIR}/../../bin/$<TARGET_FILE_NAME:neat>)
