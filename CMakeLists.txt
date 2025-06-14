cmake_minimum_required(VERSION 3.31)
project(libracing C)

set(CMAKE_C_STANDARD 11)

include_directories(include)
include_directories(src)

add_executable(libracing
        include/ac/ac.h
        include/iracing/iracing.h
        src/ac.c
        src/iracing.c)
