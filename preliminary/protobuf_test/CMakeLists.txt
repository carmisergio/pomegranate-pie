cmake_minimum_required(VERSION 3.10)

# SET(CMAKE_CXX_FLAGS "-g -Wall -Werror -std=c++11")

project(protobuf_test)

add_executable(client client.cpp)
target_link_libraries(client proto-objects asio)

add_executable(server server.cpp)
target_link_libraries(server proto-objects asio)

# Protobuf generation
find_package(protobuf CONFIG REQUIRED)

add_library(proto-objects OBJECT "${CMAKE_CURRENT_LIST_DIR}/proto/test.proto")

target_link_libraries(proto-objects PUBLIC protobuf::libprotobuf)

# set(PROTO_BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/generated")
set(PROTO_BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}")

target_include_directories(proto-objects PUBLIC "$<BUILD_INTERFACE:${PROTO_BINARY_DIR}>")

protobuf_generate(
    TARGET proto-objects
    IMPORT_DIRS "${CMAKE_CURRENT_LIST_DIR}/proto"
    PROTOC_OUT_DIR "${PROTO_BINARY_DIR}")

# Asio library
add_library(asio INTERFACE)
target_include_directories(asio INTERFACE "asio/")