# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

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
CMAKE_SOURCE_DIR = /home/yyx/ascend/competition/EyeCustom

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/yyx/ascend/competition/EyeCustom/build_out

# Utility rule file for binary.

# Include any custom commands dependencies for this target.
include op_kernel/CMakeFiles/binary.dir/compiler_depend.make

# Include the progress variables for this target.
include op_kernel/CMakeFiles/binary.dir/progress.make

binary: op_kernel/CMakeFiles/binary.dir/build.make
.PHONY : binary

# Rule to build all files generated by this target.
op_kernel/CMakeFiles/binary.dir/build: binary
.PHONY : op_kernel/CMakeFiles/binary.dir/build

op_kernel/CMakeFiles/binary.dir/clean:
	cd /home/yyx/ascend/competition/EyeCustom/build_out/op_kernel && $(CMAKE_COMMAND) -P CMakeFiles/binary.dir/cmake_clean.cmake
.PHONY : op_kernel/CMakeFiles/binary.dir/clean

op_kernel/CMakeFiles/binary.dir/depend:
	cd /home/yyx/ascend/competition/EyeCustom/build_out && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/yyx/ascend/competition/EyeCustom /home/yyx/ascend/competition/EyeCustom/op_kernel /home/yyx/ascend/competition/EyeCustom/build_out /home/yyx/ascend/competition/EyeCustom/build_out/op_kernel /home/yyx/ascend/competition/EyeCustom/build_out/op_kernel/CMakeFiles/binary.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : op_kernel/CMakeFiles/binary.dir/depend

