project(libs)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(config config/config.cpp)
target_include_directories(config PUBLIC "config/")