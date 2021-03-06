cmake_minimum_required(VERSION 3.20)
set(CMAKE_CXX_STANDARD 11)
project(pong-evolution CXX)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(DARWIN_ROOT ${PROJECT_SOURCE_DIR}/../../../..)
set(PONG_ROOT ${PROJECT_SOURCE_DIR}/../..)

if(WIN32)
  set(CMAKE_FIND_LIBRARY_SUFFIXES ".dll" ".a")
endif()

file(GLOB SRC_FILES ${PROJECT_SOURCE_DIR}/src/*.cpp)
add_library(evolution SHARED ${SRC_FILES})

target_include_directories(
  evolution
  PRIVATE ${PROJECT_SOURCE_DIR}/../../godot-cpp/godot-headers
          ${PROJECT_SOURCE_DIR}/../../godot-cpp/include/core
          ${PROJECT_SOURCE_DIR}/../../godot-cpp/include/gen
          ${PROJECT_SOURCE_DIR}/../../godot-cpp/include
          ${PROJECT_SOURCE_DIR}/include
          ${DARWIN_ROOT}/modules/persistent-models/c-headers)

# Persistent Models
find_library(PERSISTENT_MODELS_LIB persistent_models PATHS ${PONG_ROOT}/bin
                                                           REQUIRED)

# Godot CPP
find_library(GODOT_CPP_LIB godotcpp
             PATHS ${PONG_ROOT}/godot-cpp/bin REQUIRED)

# Link libraries
target_link_libraries(evolution ${GODOT_CPP_LIB} ${PERSISTENT_MODELS_LIB})

# Copy the generated library to a the "bin" directory of the extension
add_custom_command(
  TARGET evolution
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:evolution>
          ${PROJECT_SOURCE_DIR}/../../bin/$<TARGET_FILE_NAME:evolution>)
