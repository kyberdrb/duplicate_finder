cmake_minimum_required(VERSION 3.21)
project(duplicate_finder)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lcrypto -lssl")

add_executable(duplicate_finder src/main.cpp src/File.cpp src/File.h)
