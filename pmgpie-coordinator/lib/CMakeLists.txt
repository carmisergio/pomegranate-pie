project(libs)

# ASIO library
add_library(asio INTERFACE)
target_include_directories(asio INTERFACE "asio/")