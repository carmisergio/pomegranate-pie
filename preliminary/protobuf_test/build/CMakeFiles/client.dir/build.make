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
CMAKE_SOURCE_DIR = /home/sergio/dev/pomegranate-pie/preliminary/protobuf_test

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/sergio/dev/pomegranate-pie/preliminary/protobuf_test/build

# Include any dependencies generated for this target.
include CMakeFiles/client.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/client.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/client.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/client.dir/flags.make

CMakeFiles/client.dir/client.cpp.o: CMakeFiles/client.dir/flags.make
CMakeFiles/client.dir/client.cpp.o: /home/sergio/dev/pomegranate-pie/preliminary/protobuf_test/client.cpp
CMakeFiles/client.dir/client.cpp.o: CMakeFiles/client.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/sergio/dev/pomegranate-pie/preliminary/protobuf_test/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/client.dir/client.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/client.dir/client.cpp.o -MF CMakeFiles/client.dir/client.cpp.o.d -o CMakeFiles/client.dir/client.cpp.o -c /home/sergio/dev/pomegranate-pie/preliminary/protobuf_test/client.cpp

CMakeFiles/client.dir/client.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/client.dir/client.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/sergio/dev/pomegranate-pie/preliminary/protobuf_test/client.cpp > CMakeFiles/client.dir/client.cpp.i

CMakeFiles/client.dir/client.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/client.dir/client.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/sergio/dev/pomegranate-pie/preliminary/protobuf_test/client.cpp -o CMakeFiles/client.dir/client.cpp.s

# Object files for target client
client_OBJECTS = \
"CMakeFiles/client.dir/client.cpp.o"

# External object files for target client
client_EXTERNAL_OBJECTS = \
"/home/sergio/dev/pomegranate-pie/preliminary/protobuf_test/build/CMakeFiles/proto-objects.dir/test.pb.cc.o"

client: CMakeFiles/client.dir/client.cpp.o
client: CMakeFiles/proto-objects.dir/test.pb.cc.o
client: CMakeFiles/client.dir/build.make
client: /usr/lib/libprotobuf.so.25.1.0
client: /usr/lib/libabsl_log_internal_check_op.so.2308.0.0
client: /usr/lib/libabsl_leak_check.so.2308.0.0
client: /usr/lib/libabsl_die_if_null.so.2308.0.0
client: /usr/lib/libabsl_log_internal_conditions.so.2308.0.0
client: /usr/lib/libabsl_log_internal_message.so.2308.0.0
client: /usr/lib/libabsl_log_internal_nullguard.so.2308.0.0
client: /usr/lib/libabsl_examine_stack.so.2308.0.0
client: /usr/lib/libabsl_log_internal_format.so.2308.0.0
client: /usr/lib/libabsl_log_internal_proto.so.2308.0.0
client: /usr/lib/libabsl_log_internal_log_sink_set.so.2308.0.0
client: /usr/lib/libabsl_log_sink.so.2308.0.0
client: /usr/lib/libabsl_log_entry.so.2308.0.0
client: /usr/lib/libabsl_flags.so.2308.0.0
client: /usr/lib/libabsl_flags_internal.so.2308.0.0
client: /usr/lib/libabsl_flags_marshalling.so.2308.0.0
client: /usr/lib/libabsl_flags_reflection.so.2308.0.0
client: /usr/lib/libabsl_flags_config.so.2308.0.0
client: /usr/lib/libabsl_flags_program_name.so.2308.0.0
client: /usr/lib/libabsl_flags_private_handle_accessor.so.2308.0.0
client: /usr/lib/libabsl_flags_commandlineflag.so.2308.0.0
client: /usr/lib/libabsl_flags_commandlineflag_internal.so.2308.0.0
client: /usr/lib/libabsl_log_initialize.so.2308.0.0
client: /usr/lib/libabsl_log_globals.so.2308.0.0
client: /usr/lib/libabsl_log_internal_globals.so.2308.0.0
client: /usr/lib/libabsl_raw_hash_set.so.2308.0.0
client: /usr/lib/libabsl_hash.so.2308.0.0
client: /usr/lib/libabsl_city.so.2308.0.0
client: /usr/lib/libabsl_low_level_hash.so.2308.0.0
client: /usr/lib/libabsl_hashtablez_sampler.so.2308.0.0
client: /usr/lib/libabsl_statusor.so.2308.0.0
client: /usr/lib/libabsl_status.so.2308.0.0
client: /usr/lib/libabsl_cord.so.2308.0.0
client: /usr/lib/libabsl_cordz_info.so.2308.0.0
client: /usr/lib/libabsl_cord_internal.so.2308.0.0
client: /usr/lib/libabsl_cordz_functions.so.2308.0.0
client: /usr/lib/libabsl_exponential_biased.so.2308.0.0
client: /usr/lib/libabsl_cordz_handle.so.2308.0.0
client: /usr/lib/libabsl_crc_cord_state.so.2308.0.0
client: /usr/lib/libabsl_crc32c.so.2308.0.0
client: /usr/lib/libabsl_crc_internal.so.2308.0.0
client: /usr/lib/libabsl_crc_cpu_detect.so.2308.0.0
client: /usr/lib/libabsl_bad_optional_access.so.2308.0.0
client: /usr/lib/libabsl_str_format_internal.so.2308.0.0
client: /usr/lib/libabsl_strerror.so.2308.0.0
client: /usr/lib/libabsl_synchronization.so.2308.0.0
client: /usr/lib/libabsl_stacktrace.so.2308.0.0
client: /usr/lib/libabsl_symbolize.so.2308.0.0
client: /usr/lib/libabsl_debugging_internal.so.2308.0.0
client: /usr/lib/libabsl_demangle_internal.so.2308.0.0
client: /usr/lib/libabsl_graphcycles_internal.so.2308.0.0
client: /usr/lib/libabsl_kernel_timeout_internal.so.2308.0.0
client: /usr/lib/libabsl_malloc_internal.so.2308.0.0
client: /usr/lib/libabsl_time.so.2308.0.0
client: /usr/lib/libabsl_strings.so.2308.0.0
client: /usr/lib/libabsl_string_view.so.2308.0.0
client: /usr/lib/libabsl_throw_delegate.so.2308.0.0
client: /usr/lib/libabsl_strings_internal.so.2308.0.0
client: /usr/lib/libabsl_base.so.2308.0.0
client: /usr/lib/libabsl_spinlock_wait.so.2308.0.0
client: /usr/lib/libabsl_int128.so.2308.0.0
client: /usr/lib/libabsl_civil_time.so.2308.0.0
client: /usr/lib/libabsl_time_zone.so.2308.0.0
client: /usr/lib/libabsl_bad_variant_access.so.2308.0.0
client: /usr/lib/libabsl_raw_logging_internal.so.2308.0.0
client: /usr/lib/libabsl_log_severity.so.2308.0.0
client: CMakeFiles/client.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/sergio/dev/pomegranate-pie/preliminary/protobuf_test/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable client"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/client.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/client.dir/build: client
.PHONY : CMakeFiles/client.dir/build

CMakeFiles/client.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/client.dir/cmake_clean.cmake
.PHONY : CMakeFiles/client.dir/clean

CMakeFiles/client.dir/depend:
	cd /home/sergio/dev/pomegranate-pie/preliminary/protobuf_test/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/sergio/dev/pomegranate-pie/preliminary/protobuf_test /home/sergio/dev/pomegranate-pie/preliminary/protobuf_test /home/sergio/dev/pomegranate-pie/preliminary/protobuf_test/build /home/sergio/dev/pomegranate-pie/preliminary/protobuf_test/build /home/sergio/dev/pomegranate-pie/preliminary/protobuf_test/build/CMakeFiles/client.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/client.dir/depend

