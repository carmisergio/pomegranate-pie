project(app)

# Executable build
add_executable(hex2dec main.cpp)

# Link libraries
target_link_libraries(hex2dec config hex_aggregator)

# Output executabe in main build folder
set_target_properties(hex2dec PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}" )