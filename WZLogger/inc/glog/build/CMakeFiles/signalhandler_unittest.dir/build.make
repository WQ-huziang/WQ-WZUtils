# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.5

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/huangxiaolin/Desktop/Log/glog

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/huangxiaolin/Desktop/Log/glog/build

# Include any dependencies generated for this target.
include CMakeFiles/signalhandler_unittest.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/signalhandler_unittest.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/signalhandler_unittest.dir/flags.make

CMakeFiles/signalhandler_unittest.dir/src/signalhandler_unittest.cc.o: CMakeFiles/signalhandler_unittest.dir/flags.make
CMakeFiles/signalhandler_unittest.dir/src/signalhandler_unittest.cc.o: ../src/signalhandler_unittest.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/huangxiaolin/Desktop/Log/glog/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/signalhandler_unittest.dir/src/signalhandler_unittest.cc.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/signalhandler_unittest.dir/src/signalhandler_unittest.cc.o -c /home/huangxiaolin/Desktop/Log/glog/src/signalhandler_unittest.cc

CMakeFiles/signalhandler_unittest.dir/src/signalhandler_unittest.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/signalhandler_unittest.dir/src/signalhandler_unittest.cc.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/huangxiaolin/Desktop/Log/glog/src/signalhandler_unittest.cc > CMakeFiles/signalhandler_unittest.dir/src/signalhandler_unittest.cc.i

CMakeFiles/signalhandler_unittest.dir/src/signalhandler_unittest.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/signalhandler_unittest.dir/src/signalhandler_unittest.cc.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/huangxiaolin/Desktop/Log/glog/src/signalhandler_unittest.cc -o CMakeFiles/signalhandler_unittest.dir/src/signalhandler_unittest.cc.s

CMakeFiles/signalhandler_unittest.dir/src/signalhandler_unittest.cc.o.requires:

.PHONY : CMakeFiles/signalhandler_unittest.dir/src/signalhandler_unittest.cc.o.requires

CMakeFiles/signalhandler_unittest.dir/src/signalhandler_unittest.cc.o.provides: CMakeFiles/signalhandler_unittest.dir/src/signalhandler_unittest.cc.o.requires
	$(MAKE) -f CMakeFiles/signalhandler_unittest.dir/build.make CMakeFiles/signalhandler_unittest.dir/src/signalhandler_unittest.cc.o.provides.build
.PHONY : CMakeFiles/signalhandler_unittest.dir/src/signalhandler_unittest.cc.o.provides

CMakeFiles/signalhandler_unittest.dir/src/signalhandler_unittest.cc.o.provides.build: CMakeFiles/signalhandler_unittest.dir/src/signalhandler_unittest.cc.o


# Object files for target signalhandler_unittest
signalhandler_unittest_OBJECTS = \
"CMakeFiles/signalhandler_unittest.dir/src/signalhandler_unittest.cc.o"

# External object files for target signalhandler_unittest
signalhandler_unittest_EXTERNAL_OBJECTS =

signalhandler_unittest: CMakeFiles/signalhandler_unittest.dir/src/signalhandler_unittest.cc.o
signalhandler_unittest: CMakeFiles/signalhandler_unittest.dir/build.make
signalhandler_unittest: libglog.a
signalhandler_unittest: /usr/local/lib/libgflags.a
signalhandler_unittest: CMakeFiles/signalhandler_unittest.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/huangxiaolin/Desktop/Log/glog/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable signalhandler_unittest"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/signalhandler_unittest.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/signalhandler_unittest.dir/build: signalhandler_unittest

.PHONY : CMakeFiles/signalhandler_unittest.dir/build

CMakeFiles/signalhandler_unittest.dir/requires: CMakeFiles/signalhandler_unittest.dir/src/signalhandler_unittest.cc.o.requires

.PHONY : CMakeFiles/signalhandler_unittest.dir/requires

CMakeFiles/signalhandler_unittest.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/signalhandler_unittest.dir/cmake_clean.cmake
.PHONY : CMakeFiles/signalhandler_unittest.dir/clean

CMakeFiles/signalhandler_unittest.dir/depend:
	cd /home/huangxiaolin/Desktop/Log/glog/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/huangxiaolin/Desktop/Log/glog /home/huangxiaolin/Desktop/Log/glog /home/huangxiaolin/Desktop/Log/glog/build /home/huangxiaolin/Desktop/Log/glog/build /home/huangxiaolin/Desktop/Log/glog/build/CMakeFiles/signalhandler_unittest.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/signalhandler_unittest.dir/depend

