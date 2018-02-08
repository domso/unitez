cmake_minimum_required(VERSION 2.6)
project(unitez)

add_executable(unitez main.cpp)

install(TARGETS unitez RUNTIME DESTINATION bin)
