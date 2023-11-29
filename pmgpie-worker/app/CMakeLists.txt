project(app)

# Executable build
add_executable(pmgpie-worker main.cpp pmgpie_worker.cpp)

# Include headers in this folder
target_include_directories(pmgpie-worker PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# Output executabe in main build folder
set_target_properties(pmgpie-worker PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}" )