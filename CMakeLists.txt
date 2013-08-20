cmake_minimum_required(VERSION 2.8)
project(sudoku)
add_definitions(-std=c++11)
add_executable(sudoku src/main.cpp src/sudoku.cpp src/usersudoku.cpp)
