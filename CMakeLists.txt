cmake_minimum_required(VERSION 3.16)
project(Vision C)

set(CMAKE_C_STANDARD 11)


include_directories(C_Conversion)
include_directories(C_Conversion/library)

add_executable(Vision
        C_Conversion/library/linalg.h
        C_Conversion/library/vec.h
        C_Conversion/kalman.h
        C_Conversion/kalman.c
        C_Conversion/kalman_test.c)

add_executable(Routing
        C_Conversion/A_starRouting.c)

add_executable(RoutingToDriveCommands
        C_Conversion/AStartToDriveCommands.c
        C_Conversion/CommandQueue.h
        C_Conversion/CommandQueue.c)

add_executable(Kalman
        C_Conversion/kalman_combinedtest.c
        C_Conversion/library/linalg.h)

target_link_libraries(Vision m)

target_link_libraries(RoutingToDriveCommands m)

target_link_libraries(Kalman m)