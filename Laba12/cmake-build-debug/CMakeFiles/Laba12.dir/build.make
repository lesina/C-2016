# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.7

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
CMAKE_COMMAND = /home/les/Downloads/clion-2017.1/bin/cmake/bin/cmake

# The command to remove a file.
RM = /home/les/Downloads/clion-2017.1/bin/cmake/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/les/Desktop/Desktop/OOP/C-2016/Laba12

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/les/Desktop/Desktop/OOP/C-2016/Laba12/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/Laba12.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/Laba12.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Laba12.dir/flags.make

CMakeFiles/Laba12.dir/main.cpp.o: CMakeFiles/Laba12.dir/flags.make
CMakeFiles/Laba12.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/les/Desktop/Desktop/OOP/C-2016/Laba12/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/Laba12.dir/main.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Laba12.dir/main.cpp.o -c /home/les/Desktop/Desktop/OOP/C-2016/Laba12/main.cpp

CMakeFiles/Laba12.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Laba12.dir/main.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/les/Desktop/Desktop/OOP/C-2016/Laba12/main.cpp > CMakeFiles/Laba12.dir/main.cpp.i

CMakeFiles/Laba12.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Laba12.dir/main.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/les/Desktop/Desktop/OOP/C-2016/Laba12/main.cpp -o CMakeFiles/Laba12.dir/main.cpp.s

CMakeFiles/Laba12.dir/main.cpp.o.requires:

.PHONY : CMakeFiles/Laba12.dir/main.cpp.o.requires

CMakeFiles/Laba12.dir/main.cpp.o.provides: CMakeFiles/Laba12.dir/main.cpp.o.requires
	$(MAKE) -f CMakeFiles/Laba12.dir/build.make CMakeFiles/Laba12.dir/main.cpp.o.provides.build
.PHONY : CMakeFiles/Laba12.dir/main.cpp.o.provides

CMakeFiles/Laba12.dir/main.cpp.o.provides.build: CMakeFiles/Laba12.dir/main.cpp.o


# Object files for target Laba12
Laba12_OBJECTS = \
"CMakeFiles/Laba12.dir/main.cpp.o"

# External object files for target Laba12
Laba12_EXTERNAL_OBJECTS =

Laba12: CMakeFiles/Laba12.dir/main.cpp.o
Laba12: CMakeFiles/Laba12.dir/build.make
Laba12: CMakeFiles/Laba12.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/les/Desktop/Desktop/OOP/C-2016/Laba12/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable Laba12"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Laba12.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Laba12.dir/build: Laba12

.PHONY : CMakeFiles/Laba12.dir/build

CMakeFiles/Laba12.dir/requires: CMakeFiles/Laba12.dir/main.cpp.o.requires

.PHONY : CMakeFiles/Laba12.dir/requires

CMakeFiles/Laba12.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Laba12.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Laba12.dir/clean

CMakeFiles/Laba12.dir/depend:
	cd /home/les/Desktop/Desktop/OOP/C-2016/Laba12/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/les/Desktop/Desktop/OOP/C-2016/Laba12 /home/les/Desktop/Desktop/OOP/C-2016/Laba12 /home/les/Desktop/Desktop/OOP/C-2016/Laba12/cmake-build-debug /home/les/Desktop/Desktop/OOP/C-2016/Laba12/cmake-build-debug /home/les/Desktop/Desktop/OOP/C-2016/Laba12/cmake-build-debug/CMakeFiles/Laba12.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Laba12.dir/depend

