cmake_minimum_required(VERSION 3.13)
project(sdltemp)

set(CMAKE_CXX_STANDARD 11)

add_executable(sdltemp
#        src/main_showpic.cpp
        src/main_video.cpp
        src/Log.h
        )
target_link_libraries(sdltemp SDL2)