cmake_minimum_required (VERSION 3.8)

project("LinkCutTree")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_library(LinkCutTree INTERFACE)
target_include_directories(LinkCutTree INTERFACE "include/LinkCutTree")

add_library(Utils INTERFACE)
target_include_directories(Utils INTERFACE "util")

enable_testing()

add_executable(TestSplay "test/TestSplay.cpp")
target_link_libraries(TestSplay LinkCutTree)
target_link_libraries(TestSplay Utils)
add_test(TestSplayTestTarget TestSplay)

add_executable(TestAccess "test/TestAccess.cpp")
target_link_libraries(TestAccess LinkCutTree)
target_link_libraries(TestAccess Utils)
add_test(TestAccessTestTarget TestAccess)

add_executable(Demo "demo/Demo.cpp")
target_link_libraries(Demo LinkCutTree)
target_link_libraries(Demo Utils)
