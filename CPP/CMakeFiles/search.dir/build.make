# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

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
CMAKE_SOURCE_DIR = /home/fenix/CPP/HW/CPP

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/fenix/CPP/HW/CPP

# Include any dependencies generated for this target.
include CMakeFiles/search.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/search.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/search.dir/flags.make

CMakeFiles/search.dir/search.cpp.o: CMakeFiles/search.dir/flags.make
CMakeFiles/search.dir/search.cpp.o: search.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/fenix/CPP/HW/CPP/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/search.dir/search.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/search.dir/search.cpp.o -c /home/fenix/CPP/HW/CPP/search.cpp

CMakeFiles/search.dir/search.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/search.dir/search.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/fenix/CPP/HW/CPP/search.cpp > CMakeFiles/search.dir/search.cpp.i

CMakeFiles/search.dir/search.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/search.dir/search.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/fenix/CPP/HW/CPP/search.cpp -o CMakeFiles/search.dir/search.cpp.s

CMakeFiles/search.dir/search.cpp.o.requires:

.PHONY : CMakeFiles/search.dir/search.cpp.o.requires

CMakeFiles/search.dir/search.cpp.o.provides: CMakeFiles/search.dir/search.cpp.o.requires
	$(MAKE) -f CMakeFiles/search.dir/build.make CMakeFiles/search.dir/search.cpp.o.provides.build
.PHONY : CMakeFiles/search.dir/search.cpp.o.provides

CMakeFiles/search.dir/search.cpp.o.provides.build: CMakeFiles/search.dir/search.cpp.o


# Object files for target search
search_OBJECTS = \
"CMakeFiles/search.dir/search.cpp.o"

# External object files for target search
search_EXTERNAL_OBJECTS =

search: CMakeFiles/search.dir/search.cpp.o
search: CMakeFiles/search.dir/build.make
search: CMakeFiles/search.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/fenix/CPP/HW/CPP/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable search"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/search.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/search.dir/build: search

.PHONY : CMakeFiles/search.dir/build

CMakeFiles/search.dir/requires: CMakeFiles/search.dir/search.cpp.o.requires

.PHONY : CMakeFiles/search.dir/requires

CMakeFiles/search.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/search.dir/cmake_clean.cmake
.PHONY : CMakeFiles/search.dir/clean

CMakeFiles/search.dir/depend:
	cd /home/fenix/CPP/HW/CPP && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/fenix/CPP/HW/CPP /home/fenix/CPP/HW/CPP /home/fenix/CPP/HW/CPP /home/fenix/CPP/HW/CPP /home/fenix/CPP/HW/CPP/CMakeFiles/search.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/search.dir/depend

