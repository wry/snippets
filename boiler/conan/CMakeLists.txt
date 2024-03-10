cmake_minimum_required(VERSION 3.1)
project(get)

set(CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})

find_package(CURL)

add_executable(get src/main.cpp)
target_link_libraries(get CURL::CURL)
