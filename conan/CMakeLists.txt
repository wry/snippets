# cmake ..
project(get)
cmake_minimum_required(VERSION 3.1)
set(CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})
add_executable(get src/main.cpp)
find_package(CURL)
target_link_libraries(get CURL::CURL)
