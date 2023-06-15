cmake_minimum_required(VERSION 3.16)
project(pong)

set(CMAKE_CXX_STANDARD 17)

add_executable(pong main.cpp engine.cpp)
include_directories(c:\\mingw64\\include)
link_directories(c:\\mingw64\\bin)
target_link_libraries(pong mingw32 SDL2main SDL2)