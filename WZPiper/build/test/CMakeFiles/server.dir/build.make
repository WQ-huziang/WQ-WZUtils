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
include test/CMakeFiles/server.dir/depend.make

# Include the progress variables for this target.
include test/CMakeFiles/server.dir/progress.make

# Include the compile flags for this target's objects.
include test/CMakeFiles/server.dir/flags.make

test/CMakeFiles/server.dir/server.cpp.o: test/CMakeFiles/server.dir/flags.make
test/CMakeFiles/server.dir/server.cpp.o: ../test/server.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/luojunbin/project/WQ_project/WZUtils/WZPiper/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object test/CMakeFiles/server.dir/server.cpp.o"
	cd /home/luojunbin/project/WQ_project/WZUtils/WZPiper/build/test && /usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/server.dir/server.cpp.o -c /home/luojunbin/project/WQ_project/WZUtils/WZPiper/test/server.cpp

test/CMakeFiles/server.dir/server.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/server.dir/server.cpp.i"
	cd /home/luojunbin/project/WQ_project/WZUtils/WZPiper/build/test && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/luojunbin/project/WQ_project/WZUtils/WZPiper/test/server.cpp > CMakeFiles/server.dir/server.cpp.i

test/CMakeFiles/server.dir/server.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/server.dir/server.cpp.s"
	cd /home/luojunbin/project/WQ_project/WZUtils/WZPiper/build/test && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/luojunbin/project/WQ_project/WZUtils/WZPiper/test/server.cpp -o CMakeFiles/server.dir/server.cpp.s

test/CMakeFiles/server.dir/server.cpp.o.requires:

.PHONY : test/CMakeFiles/server.dir/server.cpp.o.requires

test/CMakeFiles/server.dir/server.cpp.o.provides: test/CMakeFiles/server.dir/server.cpp.o.requires
	$(MAKE) -f test/CMakeFiles/server.dir/build.make test/CMakeFiles/server.dir/server.cpp.o.provides.build
.PHONY : test/CMakeFiles/server.dir/server.cpp.o.provides

test/CMakeFiles/server.dir/server.cpp.o.provides.build: test/CMakeFiles/server.dir/server.cpp.o


# Object files for target server
server_OBJECTS = \
"CMakeFiles/server.dir/server.cpp.o"

# External object files for target server
server_EXTERNAL_OBJECTS =

../bin/server: test/CMakeFiles/server.dir/server.cpp.o
../bin/server: test/CMakeFiles/server.dir/build.make
../bin/server: ../lib/libtcp.so
../bin/server: ../lib/libwzsocket.so
../bin/server: ../lib/libudp.so
../bin/server: test/CMakeFiles/server.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/luojunbin/project/WQ_project/WZUtils/WZPiper/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ../../bin/server"
	cd /home/luojunbin/project/WQ_project/WZUtils/WZPiper/build/test && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/server.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
test/CMakeFiles/server.dir/build: ../bin/server

.PHONY : test/CMakeFiles/server.dir/build

test/CMakeFiles/server.dir/requires: test/CMakeFiles/server.dir/server.cpp.o.requires

.PHONY : test/CMakeFiles/server.dir/requires

test/CMakeFiles/server.dir/clean:
	cd /home/luojunbin/project/WQ_project/WZUtils/WZPiper/build/test && $(CMAKE_COMMAND) -P CMakeFiles/server.dir/cmake_clean.cmake
.PHONY : test/CMakeFiles/server.dir/clean

test/CMakeFiles/server.dir/depend:
	cd /home/luojunbin/project/WQ_project/WZUtils/WZPiper/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/luojunbin/project/WQ_project/WZUtils/WZPiper /home/luojunbin/project/WQ_project/WZUtils/WZPiper/test /home/luojunbin/project/WQ_project/WZUtils/WZPiper/build /home/luojunbin/project/WQ_project/WZUtils/WZPiper/build/test /home/luojunbin/project/WQ_project/WZUtils/WZPiper/build/test/CMakeFiles/server.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : test/CMakeFiles/server.dir/depend

