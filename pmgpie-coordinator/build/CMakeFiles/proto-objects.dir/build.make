# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.27

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/sergio/dev/pomegranate-pie/pmgpie-coordinator

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/sergio/dev/pomegranate-pie/pmgpie-coordinator/build

# Include any dependencies generated for this target.
include CMakeFiles/proto-objects.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/proto-objects.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/proto-objects.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/proto-objects.dir/flags.make

pmgpie_cluster.pb.h: /home/sergio/dev/pomegranate-pie/pmgpie-coordinator/proto/pmgpie_cluster.proto
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --blue --bold --progress-dir=/home/sergio/dev/pomegranate-pie/pmgpie-coordinator/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Running cpp protocol buffer compiler on /home/sergio/dev/pomegranate-pie/pmgpie-coordinator/proto/pmgpie_cluster.proto"
	/usr/bin/protoc-25.1.0 --cpp_out :/home/sergio/dev/pomegranate-pie/pmgpie-coordinator/build -I /home/sergio/dev/pomegranate-pie/pmgpie-coordinator/proto /home/sergio/dev/pomegranate-pie/pmgpie-coordinator/proto/pmgpie_cluster.proto

pmgpie_cluster.pb.cc: pmgpie_cluster.pb.h
	@$(CMAKE_COMMAND) -E touch_nocreate pmgpie_cluster.pb.cc

CMakeFiles/proto-objects.dir/pmgpie_cluster.pb.cc.o: CMakeFiles/proto-objects.dir/flags.make
CMakeFiles/proto-objects.dir/pmgpie_cluster.pb.cc.o: pmgpie_cluster.pb.cc
CMakeFiles/proto-objects.dir/pmgpie_cluster.pb.cc.o: CMakeFiles/proto-objects.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/sergio/dev/pomegranate-pie/pmgpie-coordinator/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/proto-objects.dir/pmgpie_cluster.pb.cc.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/proto-objects.dir/pmgpie_cluster.pb.cc.o -MF CMakeFiles/proto-objects.dir/pmgpie_cluster.pb.cc.o.d -o CMakeFiles/proto-objects.dir/pmgpie_cluster.pb.cc.o -c /home/sergio/dev/pomegranate-pie/pmgpie-coordinator/build/pmgpie_cluster.pb.cc

CMakeFiles/proto-objects.dir/pmgpie_cluster.pb.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/proto-objects.dir/pmgpie_cluster.pb.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/sergio/dev/pomegranate-pie/pmgpie-coordinator/build/pmgpie_cluster.pb.cc > CMakeFiles/proto-objects.dir/pmgpie_cluster.pb.cc.i

CMakeFiles/proto-objects.dir/pmgpie_cluster.pb.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/proto-objects.dir/pmgpie_cluster.pb.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/sergio/dev/pomegranate-pie/pmgpie-coordinator/build/pmgpie_cluster.pb.cc -o CMakeFiles/proto-objects.dir/pmgpie_cluster.pb.cc.s

proto-objects: CMakeFiles/proto-objects.dir/pmgpie_cluster.pb.cc.o
proto-objects: CMakeFiles/proto-objects.dir/build.make
.PHONY : proto-objects

# Rule to build all files generated by this target.
CMakeFiles/proto-objects.dir/build: proto-objects
.PHONY : CMakeFiles/proto-objects.dir/build

CMakeFiles/proto-objects.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/proto-objects.dir/cmake_clean.cmake
.PHONY : CMakeFiles/proto-objects.dir/clean

CMakeFiles/proto-objects.dir/depend: pmgpie_cluster.pb.cc
CMakeFiles/proto-objects.dir/depend: pmgpie_cluster.pb.h
	cd /home/sergio/dev/pomegranate-pie/pmgpie-coordinator/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/sergio/dev/pomegranate-pie/pmgpie-coordinator /home/sergio/dev/pomegranate-pie/pmgpie-coordinator /home/sergio/dev/pomegranate-pie/pmgpie-coordinator/build /home/sergio/dev/pomegranate-pie/pmgpie-coordinator/build /home/sergio/dev/pomegranate-pie/pmgpie-coordinator/build/CMakeFiles/proto-objects.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/proto-objects.dir/depend

