project(src)

# Configuration library
add_library(config config/config.cpp)
target_include_directories(config PUBLIC "config/")

# HEXAggregator library
add_library(hex_aggregator hex_aggregator/hex_aggregator.cpp)
target_include_directories(hex_aggregator PUBLIC "hex_aggregator/")

# Ctrl-C library
add_library(ctrl_c ctrl_c/ctrl_c.cpp)
target_include_directories(ctrl_c PUBLIC "ctrl_c/")
