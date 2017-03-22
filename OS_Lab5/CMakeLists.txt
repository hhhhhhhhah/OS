cmake_minimum_required(VERSION 3.6)
project(OS_Lab5)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES main.c Vector.h Vector.c)
add_executable(OS_Lab5 ${SOURCE_FILES})