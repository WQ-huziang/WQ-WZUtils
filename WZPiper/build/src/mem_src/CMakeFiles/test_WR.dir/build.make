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
CMAKE_SOURCE_DIR = /home/luoqingming/coding/WZUtils/WZPiper

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/luoqingming/coding/WZUtils/WZPiper/build

# Include any dependencies generated for this target.
include src/mem_src/CMakeFiles/test_WR.dir/depend.make

# Include the progress variables for this target.
include src/mem_src/CMakeFiles/test_WR.dir/progress.make

# Include the compile flags for this target's objects.
include src/mem_src/CMakeFiles/test_WR.dir/flags.make

src/mem_src/CMakeFiles/test_WR.dir/test_WR.cpp.o: src/mem_src/CMakeFiles/test_WR.dir/flags.make
src/mem_src/CMakeFiles/test_WR.dir/test_WR.cpp.o: ../src/mem_src/test_WR.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/luoqingming/coding/WZUtils/WZPiper/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/mem_src/CMakeFiles/test_WR.dir/test_WR.cpp.o"
	cd /home/luoqingming/coding/WZUtils/WZPiper/build/src/mem_src && /usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/test_WR.dir/test_WR.cpp.o -c /home/luoqingming/coding/WZUtils/WZPiper/src/mem_src/test_WR.cpp

src/mem_src/CMakeFiles/test_WR.dir/test_WR.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test_WR.dir/test_WR.cpp.i"
	cd /home/luoqingming/coding/WZUtils/WZPiper/build/src/mem_src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/luoqingming/coding/WZUtils/WZPiper/src/mem_src/test_WR.cpp > CMakeFiles/test_WR.dir/test_WR.cpp.i

src/mem_src/CMakeFiles/test_WR.dir/test_WR.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test_WR.dir/test_WR.cpp.s"
	cd /home/luoqingming/coding/WZUtils/WZPiper/build/src/mem_src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/luoqingming/coding/WZUtils/WZPiper/src/mem_src/test_WR.cpp -o CMakeFiles/test_WR.dir/test_WR.cpp.s

src/mem_src/CMakeFiles/test_WR.dir/test_WR.cpp.o.requires:

.PHONY : src/mem_src/CMakeFiles/test_WR.dir/test_WR.cpp.o.requires

src/mem_src/CMakeFiles/test_WR.dir/test_WR.cpp.o.provides: src/mem_src/CMakeFiles/test_WR.dir/test_WR.cpp.o.requires
	$(MAKE) -f src/mem_src/CMakeFiles/test_WR.dir/build.make src/mem_src/CMakeFiles/test_WR.dir/test_WR.cpp.o.provides.build
.PHONY : src/mem_src/CMakeFiles/test_WR.dir/test_WR.cpp.o.provides

src/mem_src/CMakeFiles/test_WR.dir/test_WR.cpp.o.provides.build: src/mem_src/CMakeFiles/test_WR.dir/test_WR.cpp.o


# Object files for target test_WR
test_WR_OBJECTS = \
"CMakeFiles/test_WR.dir/test_WR.cpp.o"

# External object files for target test_WR
test_WR_EXTERNAL_OBJECTS =

../bin/test_WR: src/mem_src/CMakeFiles/test_WR.dir/test_WR.cpp.o
../bin/test_WR: src/mem_src/CMakeFiles/test_WR.dir/build.make
../bin/test_WR: ../lib/libMemReader.so
../bin/test_WR: ../lib/libMemWriter.so
../bin/test_WR: ../lib/libMemEngine.so
../bin/test_WR: src/mem_src/CMakeFiles/test_WR.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/luoqingming/coding/WZUtils/WZPiper/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ../../../bin/test_WR"
	cd /home/luoqingming/coding/WZUtils/WZPiper/build/src/mem_src && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/test_WR.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/mem_src/CMakeFiles/test_WR.dir/build: ../bin/test_WR

.PHONY : src/mem_src/CMakeFiles/test_WR.dir/build

src/mem_src/CMakeFiles/test_WR.dir/requires: src/mem_src/CMakeFiles/test_WR.dir/test_WR.cpp.o.requires

.PHONY : src/mem_src/CMakeFiles/test_WR.dir/requires

src/mem_src/CMakeFiles/test_WR.dir/clean:
	cd /home/luoqingming/coding/WZUtils/WZPiper/build/src/mem_src && $(CMAKE_COMMAND) -P CMakeFiles/test_WR.dir/cmake_clean.cmake
.PHONY : src/mem_src/CMakeFiles/test_WR.dir/clean

src/mem_src/CMakeFiles/test_WR.dir/depend:
	cd /home/luoqingming/coding/WZUtils/WZPiper/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/luoqingming/coding/WZUtils/WZPiper /home/luoqingming/coding/WZUtils/WZPiper/src/mem_src /home/luoqingming/coding/WZUtils/WZPiper/build /home/luoqingming/coding/WZUtils/WZPiper/build/src/mem_src /home/luoqingming/coding/WZUtils/WZPiper/build/src/mem_src/CMakeFiles/test_WR.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/mem_src/CMakeFiles/test_WR.dir/depend

