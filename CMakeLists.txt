cmake_minimum_required(VERSION 3.31)
project(libracing C)

set(CMAKE_C_STANDARD 11)

include_directories(ac)
include_directories(iracing)

add_executable(libracing
        ac/ac.c
        ac/ac.h
        iracing/iracing.c
        iracing/iracing.h)
