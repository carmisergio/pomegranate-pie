project(libs)

# ASIO library
add_library(asio INTERFACE)
target_include_directories(asio INTERFACE "asio/")

# JSON library
add_library(json INTERFACE)
target_include_directories(json INTERFACE "json/")

# GMP Library
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR})
find_package(GMP REQUIRED)

# Seasocks library 
set(UNITTESTS OFF CACHE BOOL "Build seasocks unit tests")
set(SEASOCKS_EXAMPLE_APP OFF CACHE BOOL "Build seasocks example application")
set(DEFLATE_SUPPORT OFF CACHE BOOL "Seasocks deflate support")
add_subdirectory(seasocks)
