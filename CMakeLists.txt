cmake_minimum_required(VERSION 3.12)
project(BubbleOscillation)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-O3")

add_executable(BubbleOscillation main.cpp
                                 RayleighPlesset.h
                                 RayleighPlesset.cpp
                                 util.h
                                 util.cpp)