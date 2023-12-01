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
target_link_libraries(worker_thread pi)

# Ctrl-C library
add_library(ctrl_c ctrl_c/ctrl_c.cpp)
target_include_directories(ctrl_c PUBLIC "ctrl_c/")