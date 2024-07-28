cmake_minimum_required(VERSION 3.28)
project(HandlerThread)

set(CMAKE_CXX_STANDARD 17)

add_executable(HandlerThread main.cpp
        Message.cpp
        Message.h
        MessageQueue.cpp
        MessageQueue.h
        vtime.h
        Looper.cpp
        Looper.h
        HandlerThread.cpp
        HandlerThread.h)
