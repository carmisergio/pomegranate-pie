# Project configuration
cmake_minimum_required(VERSION 3.10)
project(hex2dec VERSION 0.0)

# Compilation settings
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")

# Source subdirectory
add_subdirectory(app)
add_subdirectory(src)