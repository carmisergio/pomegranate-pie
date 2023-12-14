project(src)

# Configuration library
add_library(config config/config.cpp)
target_include_directories(config PUBLIC "config/")

# HEXAggregator 
add_library(hex_aggregator hex_aggregator/hex_aggregator.cpp)
target_include_directories(hex_aggregator PUBLIC "hex_aggregator/")
target_link_libraries(hex_aggregator gmp gmpxx)