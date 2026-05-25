# cmake -G Xcode -DCMAKE_BUILD_TYPE=Debug ..

cmake_minimum_required(VERSION 3.10)
project(ardec)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_OSX_DEPLOYMENT_TARGET "26.4.1" CACHE STRING "Minimum OS X deployment version" FORCE)

file(GLOB_RECURSE src
  src/*.m
  src/*.h
) 

add_executable(ardec ${src})

set_target_properties(ardec PROPERTIES
  XCODE_ATTRIBUTE_CLANG_ENABLE_OBJC_ARC YES
)

target_compile_options(ardec PUBLIC "-fobjc-arc")
target_compile_definitions(ardec PRIVATE
    $<$<CONFIG:Debug>:
        DEBUG=1
    >
)

target_include_directories(ardec PUBLIC
  src/
)

target_link_libraries(ardec PUBLIC
  "-framework Foundation"
  objc
)

