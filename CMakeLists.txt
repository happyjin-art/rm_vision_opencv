cmake_minimum_required(VERSION 3.10)
project(rm_vision)


set(CMAKE_CXX_STANDARD 11)


find_package(OpenCV REQUIRED)


include_directories(${OpenCV_INCLUDE_DIRS})


add_executable(rm_vision main.cpp)


target_link_libraries(rm_vision ${OpenCV_LIBS})