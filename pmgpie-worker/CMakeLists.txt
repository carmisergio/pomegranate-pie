# Project configuration
cmake_minimum_required(VERSION 3.10)
project(pmgpie-worker VERSION 0.0)

# Compilation settings
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")

# Source subdirectory
add_subdirectory(app)
add_subdirectory(src)
add_subdirectory(lib)

# Protobuf generation
find_package(protobuf CONFIG REQUIRED)
add_library(proto-objects OBJECT "${CMAKE_CURRENT_LIST_DIR}/proto/pmgpie_cluster.proto")
target_link_libraries(proto-objects PUBLIC protobuf::libprotobuf)
# set(PROTO_BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/generated")
set(PROTO_BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}")
target_include_directories(proto-objects PUBLIC "$<BUILD_INTERFACE:${PROTO_BINARY_DIR}>")
protobuf_generate(
    TARGET proto-objects
    IMPORT_DIRS "${CMAKE_CURRENT_LIST_DIR}/proto"
    PROTOC_OUT_DIR "${PROTO_BINARY_DIR}")