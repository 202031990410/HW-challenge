cmake_minimum_required(VERSION 3.30)
project(ChallengeGame)

set(CMAKE_CXX_STANDARD 20)

add_executable(ChallengeGame demo.cpp
        Request.cpp
        Request.h
        Object.cpp
        Object.h
        bucket.cpp
        bucket.h
        Disk.cpp
        Disk.h
        Label.cpp
        Label.h
        BuckeSet.cpp
        BuckeSet.h
        main.cpp)
