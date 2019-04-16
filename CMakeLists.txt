cmake_minimum_required(VERSION 3.13)
project(Operating_Systems C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(Operating_Systems
        commands.c
        commands.h
        list.c
        list.h
        process.c
        process.h
        signals.c
        signals.h
        smash.c)
