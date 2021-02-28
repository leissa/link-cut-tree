cmake_minimum_required (VERSION 3.8)

project("LinkCutTree")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_library(LinkCutTree INTERFACE)
target_include_directories(LinkCutTree INTERFACE "include/LinkCutTree")

add_executable(Demo "demo/Demo.cpp")
target_link_libraries(Demo LinkCutTree)
