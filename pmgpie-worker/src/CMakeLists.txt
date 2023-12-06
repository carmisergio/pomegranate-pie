project(src)

# Configuration library
add_library(config config/config.cpp)
target_include_directories(config PUBLIC "config/")

# Pi computation library
add_library(pi pi/pi.cpp)
target_include_directories(pi PUBLIC "pi/")

# Worker Thread object
add_library(worker_pool worker_pool/worker_thread.cpp worker_pool/worker_thread_pool.cpp)
target_include_directories(worker_pool PUBLIC "worker_pool/")
target_link_libraries(worker_pool pi)

# Ctrl-C library
add_library(ctrl_c ctrl_c/ctrl_c.cpp)
target_include_directories(ctrl_c PUBLIC "ctrl_c/")

# PMGPIe Cluster Client
add_library(net_client net_client/net_client.cpp)
target_include_directories(net_client PUBLIC "net_client/")
target_link_libraries(net_client asio proto-objects worker_pool)