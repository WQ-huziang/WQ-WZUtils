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
CMAKE_SOURCE_DIR = /home/luojunbin/project/WQ_project/WZUtils/WZPiper

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/luojunbin/project/WQ_project/WZUtils/WZPiper/build

# Include any dependencies generated for this target.
include src/mem_src/CMakeFiles/gtest_memqueue.dir/depend.make

# Include the progress variables for this target.
include src/mem_src/CMakeFiles/gtest_memqueue.dir/progress.make

# Include the compile flags for this target's objects.
include src/mem_src/CMakeFiles/gtest_memqueue.dir/flags.make

src/mem_src/CMakeFiles/gtest_memqueue.dir/gtest_memqueue.cpp.o: src/mem_src/CMakeFiles/gtest_memqueue.dir/flags.make
src/mem_src/CMakeFiles/gtest_memqueue.dir/gtest_memqueue.cpp.o: ../src/mem_src/gtest_memqueue.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/luojunbin/project/WQ_project/WZUtils/WZPiper/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/mem_src/CMakeFiles/gtest_memqueue.dir/gtest_memqueue.cpp.o"
	cd /home/luojunbin/project/WQ_project/WZUtils/WZPiper/build/src/mem_src && /usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/gtest_memqueue.dir/gtest_memqueue.cpp.o -c /home/luojunbin/project/WQ_project/WZUtils/WZPiper/src/mem_src/gtest_memqueue.cpp

src/mem_src/CMakeFiles/gtest_memqueue.dir/gtest_memqueue.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/gtest_memqueue.dir/gtest_memqueue.cpp.i"
	cd /home/luojunbin/project/WQ_project/WZUtils/WZPiper/build/src/mem_src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/luojunbin/project/WQ_project/WZUtils/WZPiper/src/mem_src/gtest_memqueue.cpp > CMakeFiles/gtest_memqueue.dir/gtest_memqueue.cpp.i

src/mem_src/CMakeFiles/gtest_memqueue.dir/gtest_memqueue.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/gtest_memqueue.dir/gtest_memqueue.cpp.s"
	cd /home/luojunbin/project/WQ_project/WZUtils/WZPiper/build/src/mem_src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/luojunbin/project/WQ_project/WZUtils/WZPiper/src/mem_src/gtest_memqueue.cpp -o CMakeFiles/gtest_memqueue.dir/gtest_memqueue.cpp.s

src/mem_src/CMakeFiles/gtest_memqueue.dir/gtest_memqueue.cpp.o.requires:

.PHONY : src/mem_src/CMakeFiles/gtest_memqueue.dir/gtest_memqueue.cpp.o.requires

src/mem_src/CMakeFiles/gtest_memqueue.dir/gtest_memqueue.cpp.o.provides: src/mem_src/CMakeFiles/gtest_memqueue.dir/gtest_memqueue.cpp.o.requires
	$(MAKE) -f src/mem_src/CMakeFiles/gtest_memqueue.dir/build.make src/mem_src/CMakeFiles/gtest_memqueue.dir/gtest_memqueue.cpp.o.provides.build
.PHONY : src/mem_src/CMakeFiles/gtest_memqueue.dir/gtest_memqueue.cpp.o.provides

src/mem_src/CMakeFiles/gtest_memqueue.dir/gtest_memqueue.cpp.o.provides.build: src/mem_src/CMakeFiles/gtest_memqueue.dir/gtest_memqueue.cpp.o


# Object files for target gtest_memqueue
gtest_memqueue_OBJECTS = \
"CMakeFiles/gtest_memqueue.dir/gtest_memqueue.cpp.o"

# External object files for target gtest_memqueue
gtest_memqueue_EXTERNAL_OBJECTS =

../bin/gtest_memqueue: src/mem_src/CMakeFiles/gtest_memqueue.dir/gtest_memqueue.cpp.o
../bin/gtest_memqueue: src/mem_src/CMakeFiles/gtest_memqueue.dir/build.make
../bin/gtest_memqueue: src/mem_src/CMakeFiles/gtest_memqueue.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/luojunbin/project/WQ_project/WZUtils/WZPiper/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ../../../bin/gtest_memqueue"
	cd /home/luojunbin/project/WQ_project/WZUtils/WZPiper/build/src/mem_src && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/gtest_memqueue.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/mem_src/CMakeFiles/gtest_memqueue.dir/build: ../bin/gtest_memqueue

.PHONY : src/mem_src/CMakeFiles/gtest_memqueue.dir/build

src/mem_src/CMakeFiles/gtest_memqueue.dir/requires: src/mem_src/CMakeFiles/gtest_memqueue.dir/gtest_memqueue.cpp.o.requires

.PHONY : src/mem_src/CMakeFiles/gtest_memqueue.dir/requires

src/mem_src/CMakeFiles/gtest_memqueue.dir/clean:
	cd /home/luojunbin/project/WQ_project/WZUtils/WZPiper/build/src/mem_src && $(CMAKE_COMMAND) -P CMakeFiles/gtest_memqueue.dir/cmake_clean.cmake
.PHONY : src/mem_src/CMakeFiles/gtest_memqueue.dir/clean

src/mem_src/CMakeFiles/gtest_memqueue.dir/depend:
	cd /home/luojunbin/project/WQ_project/WZUtils/WZPiper/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/luojunbin/project/WQ_project/WZUtils/WZPiper /home/luojunbin/project/WQ_project/WZUtils/WZPiper/src/mem_src /home/luojunbin/project/WQ_project/WZUtils/WZPiper/build /home/luojunbin/project/WQ_project/WZUtils/WZPiper/build/src/mem_src /home/luojunbin/project/WQ_project/WZUtils/WZPiper/build/src/mem_src/CMakeFiles/gtest_memqueue.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/mem_src/CMakeFiles/gtest_memqueue.dir/depend

