cmake_minimum_required(VERSION 3.20)
project(CourseWork)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -lcurl")

set(SOURCE_FILES main.cpp)
add_executable(CourseWork ${SOURCE_FILES})
target_link_libraries(CourseWork curl)
