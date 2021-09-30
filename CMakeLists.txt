cmake_minimum_required(VERSION 3.20)
project(csci_460_hw2_project)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS -pthread)
add_executable(csci_460_hw2_project main.cpp LinkedList.cpp)
