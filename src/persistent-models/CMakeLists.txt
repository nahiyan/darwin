cmake_minimum_required(VERSION 3.18)

set(CMAKE_CXX_STANDARD 11)
set(APP_NAME pm)
project(pm)
set(SOURCES src/main.cpp)

if(NOT DEFINED BUNDLE)
  set(BUNDLE ON)
endif()

if(APPLE)
  if(NOT BUNDLE)
    add_library(${APP_NAME} SHARED ${SOURCES})
  else()
    add_library(${APP_NAME} MODULE ${SOURCES})
    set_target_properties(${APP_NAME} PROPERTIES BUNDLE TRUE)
  endif()
else()
  add_library(${APP_NAME} SHARED ${SOURCES})
endif()

target_include_directories(
  ${APP_NAME}
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/persistent-models/persistent-models
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/persistent-models)
