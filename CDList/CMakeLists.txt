cmake_minimum_required(VERSION 3.17)
project(CDList_2b)

set(CMAKE_CXX_STANDARD 17)

include_directories($ENV{HOME}/Programming/include)
link_directories($ENV{HOME}/Programming/lib)
link_libraries(libdataStructures.a)

add_executable(CDList_2b main.cpp CDList.h)