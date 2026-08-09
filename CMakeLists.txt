cmake_minimum_required(VERSION 3.25)
project(NetScope VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDART 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20 -Wall -Werror -Wextra -Wpedantic -g")

set(SOURCES src/main.cpp)

add_executable(NetScope ${SOURCES})
