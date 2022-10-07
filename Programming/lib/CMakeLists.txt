cmake_minimum_required(VERSION 3.14)
project(Project2)

set(CMAKE_CXX_STANDARD 17)

# add these next three lines to your CMakeLists.txt file to access the Programming folder files

include_directories($ENV{HOME}/Programming/include)
link_directories($ENV{HOME}/Programming/lib)
link_libraries(dataStructures)

add_executable(Project2 main.cpp dictionary.cpp dictionary.h fraction.cc fraction.h)