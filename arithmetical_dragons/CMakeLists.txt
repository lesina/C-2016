cmake_minimum_required(VERSION 3.6)
project(arithmetical_dragons)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES src/arithmetical_dragons.cpp src/dragon.h src/hero.h src/unit.h)
add_executable(arithmetical_dragons ${SOURCE_FILES})