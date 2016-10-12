cmake_minimum_required(VERSION 3.6)
project(Closed_Hash_table)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp)
add_executable(Closed_Hash_table ${SOURCE_FILES})