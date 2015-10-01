cmake_minimum_required (VERSION 2.8)
project(cargo C)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -pedantic")

add_executable(wordexp_test wordexp_test.c)
