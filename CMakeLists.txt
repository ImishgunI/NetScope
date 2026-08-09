cmake_minimum_required(VERSION 3.25)
project(NetScope VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -Wextra -Wpedantic -g")


set(HEADERS src/util/sum.hpp)
set(SOURCES src/util/sum.cpp)

set(TEST_SOURCES tests/main_test.cpp tests/sum_test.cpp tests/sum_test.hpp)

add_executable(NetScope src/main.cpp ${SOURCES} ${HEADERS})

enable_testing()

add_executable(NetScopeTest ${TEST_SOURCES} ${SOURCES} ${HEADERS})

add_test(NAME UnitTests COMMAND NetScopeTest)