cmake_minimum_required(VERSION 3.22)
project(initial)

set(CMAKE_CXX_STANDARD 11)

include_directories(.)

add_executable(initial
        constants.h
        errors.h
        main.cpp
        StackFrame.cpp
        StackFrame.h)
