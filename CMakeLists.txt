cmake_minimum_required (VERSION 3.8)

project("Link-Cut-Tree")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_library(Link-Cut-Tree "src/Link-Cut-Tree.cpp")
target_include_directories(Link-Cut-Tree PUBLIC "include/Link-Cut-Tree")

add_executable(Demo "demo/Demo.cpp")
target_link_libraries(Demo PUBLIC Link-Cut-Tree)
