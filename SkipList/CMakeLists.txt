cmake_minimum_required(VERSION 3.29)
project(SkipList)

set(CMAKE_CXX_STANDARD 20)

add_executable(SkipList test.cpp
        Node/Node.cpp
        Node/Node.h
        Vector/Vector.cpp
        Vector/Vector.h
        SkipList.cpp
        SkipList.h
        ResultWrapper/ResultWrapper.cpp
        ResultWrapper/ResultWrapper.h)

INCLUDE_DIRECTORIES(./)