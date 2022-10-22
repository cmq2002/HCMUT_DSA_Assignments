cmake_minimum_required(VERSION 3.21)
project(initial_original)

set(CMAKE_CXX_STANDARD 11)

include_directories(.)

add_executable(initial_original
        constants.h
        errors.h
        main.cpp
        StackFrame.cpp
        StackFrame.h)
