# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.6

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /home/ayy/Downloads/CLion-2016.3.3/clion-2016.3.3/bin/cmake/bin/cmake

# The command to remove a file.
RM = /home/ayy/Downloads/CLion-2016.3.3/clion-2016.3.3/bin/cmake/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/ayy/CLionProjects/OS/OS_Lab5

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/ayy/CLionProjects/OS/OS_Lab5/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/OS_Lab5.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/OS_Lab5.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/OS_Lab5.dir/flags.make

CMakeFiles/OS_Lab5.dir/main.c.o: CMakeFiles/OS_Lab5.dir/flags.make
CMakeFiles/OS_Lab5.dir/main.c.o: ../main.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ayy/CLionProjects/OS/OS_Lab5/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/OS_Lab5.dir/main.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/OS_Lab5.dir/main.c.o   -c /home/ayy/CLionProjects/OS/OS_Lab5/main.c

CMakeFiles/OS_Lab5.dir/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/OS_Lab5.dir/main.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/ayy/CLionProjects/OS/OS_Lab5/main.c > CMakeFiles/OS_Lab5.dir/main.c.i

CMakeFiles/OS_Lab5.dir/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/OS_Lab5.dir/main.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/ayy/CLionProjects/OS/OS_Lab5/main.c -o CMakeFiles/OS_Lab5.dir/main.c.s

CMakeFiles/OS_Lab5.dir/main.c.o.requires:

.PHONY : CMakeFiles/OS_Lab5.dir/main.c.o.requires

CMakeFiles/OS_Lab5.dir/main.c.o.provides: CMakeFiles/OS_Lab5.dir/main.c.o.requires
	$(MAKE) -f CMakeFiles/OS_Lab5.dir/build.make CMakeFiles/OS_Lab5.dir/main.c.o.provides.build
.PHONY : CMakeFiles/OS_Lab5.dir/main.c.o.provides

CMakeFiles/OS_Lab5.dir/main.c.o.provides.build: CMakeFiles/OS_Lab5.dir/main.c.o


# Object files for target OS_Lab5
OS_Lab5_OBJECTS = \
"CMakeFiles/OS_Lab5.dir/main.c.o"

# External object files for target OS_Lab5
OS_Lab5_EXTERNAL_OBJECTS =

OS_Lab5: CMakeFiles/OS_Lab5.dir/main.c.o
OS_Lab5: CMakeFiles/OS_Lab5.dir/build.make
OS_Lab5: CMakeFiles/OS_Lab5.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/ayy/CLionProjects/OS/OS_Lab5/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable OS_Lab5"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/OS_Lab5.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/OS_Lab5.dir/build: OS_Lab5

.PHONY : CMakeFiles/OS_Lab5.dir/build

CMakeFiles/OS_Lab5.dir/requires: CMakeFiles/OS_Lab5.dir/main.c.o.requires

.PHONY : CMakeFiles/OS_Lab5.dir/requires

CMakeFiles/OS_Lab5.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/OS_Lab5.dir/cmake_clean.cmake
.PHONY : CMakeFiles/OS_Lab5.dir/clean

CMakeFiles/OS_Lab5.dir/depend:
	cd /home/ayy/CLionProjects/OS/OS_Lab5/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/ayy/CLionProjects/OS/OS_Lab5 /home/ayy/CLionProjects/OS/OS_Lab5 /home/ayy/CLionProjects/OS/OS_Lab5/cmake-build-debug /home/ayy/CLionProjects/OS/OS_Lab5/cmake-build-debug /home/ayy/CLionProjects/OS/OS_Lab5/cmake-build-debug/CMakeFiles/OS_Lab5.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/OS_Lab5.dir/depend

