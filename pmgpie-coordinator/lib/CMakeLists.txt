project(libs)

# ASIO library
add_library(asio INTERFACE)
target_include_directories(asio INTERFACE "asio/")

# GMP Library
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR})
find_package(GMP REQUIRED)