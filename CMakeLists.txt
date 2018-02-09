cmake_minimum_required(VERSION 2.6)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

project(unitez)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FALGS} -O3 -std=c++17 -pthread")
add_executable(unitez unitez/unitez.cpp demo/unit_test_list.cpp)

install(TARGETS unitez RUNTIME DESTINATION bin)
