cmake_minimum_required(VERSION 3.25.1)
project(matrix)

set(CMAKE_CXX_STANDARD 20)

add_executable(matrix main.cpp 
        matrix.hpp matrix.cpp)
