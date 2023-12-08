project(libs)

# ASIO library
add_library(asio INTERFACE)
target_include_directories(asio INTERFACE "asio/")

# using pkg-config to configure uuid
# find_package(PkgConfig REQUIRED)

# UUID library
add_library(stduuid INTERFACE)
target_include_directories(stduuid INTERFACE "uuid/")

if(LINUX)
    target_link_libraries(stduuid INTERFACE uuid)
endif()