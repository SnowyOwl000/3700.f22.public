cmake_minimum_required(VERSION 3.14)
project(dynamic)

set(CMAKE_CXX_STANDARD 17)

add_executable(dynamic main.cpp mrs.cpp mrs.h sampler.cpp sampler.h)