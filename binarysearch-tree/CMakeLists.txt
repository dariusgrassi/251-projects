cmake_minimum_required(VERSION 3.14)
project(Project2)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(Project2
        bst.h
        main.cpp
        cmake-build-debug/movies1.txt
        cmake-build-debug/reviews1.txt)
