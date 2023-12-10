project(src)

# Configuration library
add_library(config config/config.cpp)
target_include_directories(config PUBLIC "config/")

# HEXAggregator library
add_library(hex_aggregator hex_aggregator/hex_aggregator.cpp)
target_include_directories(hex_aggregator PUBLIC "hex_aggregator/")
target_link_libraries(hex_aggregator gmp gmpxx)

# Ctrl-C library
add_library(ctrl_c ctrl_c/ctrl_c.cpp)
target_include_directories(ctrl_c PUBLIC "ctrl_c/")

# TCP Server library
add_library(tcp_server INTERFACE)
target_include_directories(tcp_server INTERFACE "tcp_server/")
target_link_libraries(tcp_server INTERFACE tsqueue asio)

# PMGPieClusterServer library
add_library(pmgpie_cluster_server INTERFACE)
target_include_directories(pmgpie_cluster_server INTERFACE "pmgpie_cluster_server/")
target_link_libraries(pmgpie_cluster_server INTERFACE tcp_server proto-objects work_unit_manager stats)

# WorkUnitManager library
add_library(work_unit_manager work_unit_manager/work_unit_manager.cpp)
target_include_directories(work_unit_manager PUBLIC "work_unit_manager/")
target_link_libraries(work_unit_manager work_unit_combiner hex_aggregator)

# WorkUnitCombiner library
add_library(work_unit_combiner work_unit_combiner/work_unit_combiner.cpp)
target_include_directories(work_unit_combiner PUBLIC "work_unit_combiner/")
target_link_libraries(work_unit_combiner file stats)

# FileWriter library
add_library(file file/file_writer.cpp)
target_include_directories(file PUBLIC "file/")

# Frontend library
add_library(frontend frontend/frontend.cpp)
target_include_directories(frontend PUBLIC "frontend/")
target_link_libraries(frontend seasocks stats json)

# Stats library
add_library(stats INTERFACE)
target_include_directories(stats INTERFACE "stats/")
target_link_libraries(stats INTERFACE)

# TSQueue
add_library(tsqueue INTERFACE)
target_include_directories(tsqueue INTERFACE "tsqueue/")