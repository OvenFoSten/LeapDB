cmake_minimum_required(VERSION 3.29)
project(SkipList)

set(CMAKE_CXX_STANDARD 20)

add_executable(SkipList test.cpp
        LinkList/LinkList.cpp
        LinkList/LinkList.h
        LinkList/Node/Node.cpp
        LinkList/Node/Node.h
        Vector/Vector.cpp
        Vector/Vector.h)

INCLUDE_DIRECTORIES(./)