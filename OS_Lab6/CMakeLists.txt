cmake_minimum_required(VERSION 3.6)
project(OS_Lab6)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES main_server.c List.h List.c main_client.c)
add_executable(OS_Lab6 ${SOURCE_FILES})