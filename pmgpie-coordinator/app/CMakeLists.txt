project(app)

# Executable build
add_executable(pmgpie-coordinator main.cpp pmgpie_coordinator.cpp)


# Include headers in this folder
target_include_directories(pmgpie-coordinator PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# Link libraries
target_link_libraries(pmgpie-coordinator config ctrl_c pmgpie_cluster_server proto-objects work_unit_manager work_unit_combiner file)

# Output executabe in main build folder
set_target_properties(pmgpie-coordinator PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}" )