cmake_minimum_required(VERSION 3.16)
project(elevator C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(elevator
        main.c
        elevator.h
        elevator.c
        list.c
        list.h)
