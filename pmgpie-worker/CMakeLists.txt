# Project configuration
cmake_minimum_required(VERSION 3.10)
project(pmgpie-worker VERSION 0.0)

# Compilation settings
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O3 -static-libstdc++")

# Source subdirectory
add_subdirectory(app)