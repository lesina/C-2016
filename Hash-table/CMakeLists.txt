cmake_minimum_required(VERSION 3.6)
project(Hash_table)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp doubleList.cpp doubleList.h)
add_executable(Hash_table ${SOURCE_FILES})