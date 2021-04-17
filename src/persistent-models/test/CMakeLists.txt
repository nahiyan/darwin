cmake_minimum_required(VERSION 3.18)

project(april11-rust)
add_executable(main main.cpp)
target_link_directories(main PRIVATE
                        ${CMAKE_CURRENT_SOURCE_DIR}/../target/debug)
target_link_libraries(main persistent_models)
