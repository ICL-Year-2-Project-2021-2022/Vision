cmake_minimum_required(VERSION 3.16)
project(Kalman C)

set(CMAKE_C_STANDARD 11)

include_directories(C_Conversion/library)

add_executable(Kalman
        C_Conversion/library/vec.h
        C_Conversion/kalman.h
        C_Conversion/kalman.c)
target_link_libraries(Kalman m)
