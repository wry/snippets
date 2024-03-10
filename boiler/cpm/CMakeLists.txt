cmake_minimum_required(VERSION 3.14 FATAL_ERROR)
project(CPMTest)

set(CMAKE_CXX_STANDARD 11)

include(cmake/get_cpm.cmake)
CPMAddPackage(
  NAME nlohmann_json
  GITHUB_REPOSITORY nlohmann/json
  VERSION 3.9.1
  OPTIONS 
    "JSON_BuildTests OFF"
)

add_executable(cpmtest src/main.cpp)
target_link_libraries(cpmtest PRIVATE nlohmann_json)
