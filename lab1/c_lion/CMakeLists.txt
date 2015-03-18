cmake_minimum_required(VERSION 3.1)
project(c_lion)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    solutions/first/pi_client.c
    solutions/second/file_client.c)

add_executable(c_lion ${SOURCE_FILES})