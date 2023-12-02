project(libs)

# Configuration library
add_library(config config/config.cpp)
target_include_directories(config PUBLIC "config/")

# Pi computation library
add_library(pi pi/pi.cpp)
target_include_directories(pi PUBLIC "pi/")

# Worker Thread object
add_library(worker_thread worker_thread/worker_thread.cpp)
target_include_directories(pi PUBLIC "worker_thread/")
target_link_libraries(worker_thread pi net_client)

# NetworkClient library
add_library(net_client net_client/net_client.cpp)
target_include_directories(net_client PUBLIC "net_client/")
target_link_libraries(net_client asio worker_thread)

# Ctrl-C library
add_library(ctrl_c ctrl_c/ctrl_c.cpp)
target_include_directories(ctrl_c PUBLIC "ctrl_c/")

# Asio library
add_library(asio INTERFACE)
target_include_directories(asio INTERFACE "asio/")
