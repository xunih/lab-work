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
CMAKE_COMMAND = /opt/homebrew/Cellar/cmake/3.22.3/bin/cmake

# The command to remove a file.
RM = /opt/homebrew/Cellar/cmake/3.22.3/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/xuni/Documents/Interactive_Media_Technology_MSc/DH2323/lab3/part3

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/xuni/Documents/Interactive_Media_Technology_MSc/DH2323/lab3/part3

# Include any dependencies generated for this target.
include CMakeFiles/DgiLab.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/DgiLab.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/DgiLab.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/DgiLab.dir/flags.make

CMakeFiles/DgiLab.dir/skeleton.cpp.o: CMakeFiles/DgiLab.dir/flags.make
CMakeFiles/DgiLab.dir/skeleton.cpp.o: skeleton.cpp
CMakeFiles/DgiLab.dir/skeleton.cpp.o: CMakeFiles/DgiLab.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/xuni/Documents/Interactive_Media_Technology_MSc/DH2323/lab3/part3/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/DgiLab.dir/skeleton.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/DgiLab.dir/skeleton.cpp.o -MF CMakeFiles/DgiLab.dir/skeleton.cpp.o.d -o CMakeFiles/DgiLab.dir/skeleton.cpp.o -c /Users/xuni/Documents/Interactive_Media_Technology_MSc/DH2323/lab3/part3/skeleton.cpp

CMakeFiles/DgiLab.dir/skeleton.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/DgiLab.dir/skeleton.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/xuni/Documents/Interactive_Media_Technology_MSc/DH2323/lab3/part3/skeleton.cpp > CMakeFiles/DgiLab.dir/skeleton.cpp.i

CMakeFiles/DgiLab.dir/skeleton.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/DgiLab.dir/skeleton.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/xuni/Documents/Interactive_Media_Technology_MSc/DH2323/lab3/part3/skeleton.cpp -o CMakeFiles/DgiLab.dir/skeleton.cpp.s

CMakeFiles/DgiLab.dir/SDL2Auxiliary.cpp.o: CMakeFiles/DgiLab.dir/flags.make
CMakeFiles/DgiLab.dir/SDL2Auxiliary.cpp.o: SDL2Auxiliary.cpp
CMakeFiles/DgiLab.dir/SDL2Auxiliary.cpp.o: CMakeFiles/DgiLab.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/xuni/Documents/Interactive_Media_Technology_MSc/DH2323/lab3/part3/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/DgiLab.dir/SDL2Auxiliary.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/DgiLab.dir/SDL2Auxiliary.cpp.o -MF CMakeFiles/DgiLab.dir/SDL2Auxiliary.cpp.o.d -o CMakeFiles/DgiLab.dir/SDL2Auxiliary.cpp.o -c /Users/xuni/Documents/Interactive_Media_Technology_MSc/DH2323/lab3/part3/SDL2Auxiliary.cpp

CMakeFiles/DgiLab.dir/SDL2Auxiliary.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/DgiLab.dir/SDL2Auxiliary.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/xuni/Documents/Interactive_Media_Technology_MSc/DH2323/lab3/part3/SDL2Auxiliary.cpp > CMakeFiles/DgiLab.dir/SDL2Auxiliary.cpp.i

CMakeFiles/DgiLab.dir/SDL2Auxiliary.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/DgiLab.dir/SDL2Auxiliary.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/xuni/Documents/Interactive_Media_Technology_MSc/DH2323/lab3/part3/SDL2Auxiliary.cpp -o CMakeFiles/DgiLab.dir/SDL2Auxiliary.cpp.s

# Object files for target DgiLab
DgiLab_OBJECTS = \
"CMakeFiles/DgiLab.dir/skeleton.cpp.o" \
"CMakeFiles/DgiLab.dir/SDL2Auxiliary.cpp.o"

# External object files for target DgiLab
DgiLab_EXTERNAL_OBJECTS =

DgiLab: CMakeFiles/DgiLab.dir/skeleton.cpp.o
DgiLab: CMakeFiles/DgiLab.dir/SDL2Auxiliary.cpp.o
DgiLab: CMakeFiles/DgiLab.dir/build.make
DgiLab: CMakeFiles/DgiLab.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/xuni/Documents/Interactive_Media_Technology_MSc/DH2323/lab3/part3/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable DgiLab"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/DgiLab.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/DgiLab.dir/build: DgiLab
.PHONY : CMakeFiles/DgiLab.dir/build

CMakeFiles/DgiLab.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/DgiLab.dir/cmake_clean.cmake
.PHONY : CMakeFiles/DgiLab.dir/clean

CMakeFiles/DgiLab.dir/depend:
	cd /Users/xuni/Documents/Interactive_Media_Technology_MSc/DH2323/lab3/part3 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/xuni/Documents/Interactive_Media_Technology_MSc/DH2323/lab3/part3 /Users/xuni/Documents/Interactive_Media_Technology_MSc/DH2323/lab3/part3 /Users/xuni/Documents/Interactive_Media_Technology_MSc/DH2323/lab3/part3 /Users/xuni/Documents/Interactive_Media_Technology_MSc/DH2323/lab3/part3 /Users/xuni/Documents/Interactive_Media_Technology_MSc/DH2323/lab3/part3/CMakeFiles/DgiLab.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/DgiLab.dir/depend

