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
CMAKE_SOURCE_DIR = /home/murii/Desktop/Programming/CLove

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/murii/Desktop/Programming/CLove/build

# Include any dependencies generated for this target.
include src/3rdparty/SDL2/CMakeFiles/SDL2main.dir/depend.make

# Include the progress variables for this target.
include src/3rdparty/SDL2/CMakeFiles/SDL2main.dir/progress.make

# Include the compile flags for this target's objects.
include src/3rdparty/SDL2/CMakeFiles/SDL2main.dir/flags.make

src/3rdparty/SDL2/CMakeFiles/SDL2main.dir/src/main/dummy/SDL_dummy_main.c.o: src/3rdparty/SDL2/CMakeFiles/SDL2main.dir/flags.make
src/3rdparty/SDL2/CMakeFiles/SDL2main.dir/src/main/dummy/SDL_dummy_main.c.o: ../src/3rdparty/SDL2/src/main/dummy/SDL_dummy_main.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/murii/Desktop/Programming/CLove/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object src/3rdparty/SDL2/CMakeFiles/SDL2main.dir/src/main/dummy/SDL_dummy_main.c.o"
	cd /home/murii/Desktop/Programming/CLove/build/src/3rdparty/SDL2 && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/SDL2main.dir/src/main/dummy/SDL_dummy_main.c.o   -c /home/murii/Desktop/Programming/CLove/src/3rdparty/SDL2/src/main/dummy/SDL_dummy_main.c

src/3rdparty/SDL2/CMakeFiles/SDL2main.dir/src/main/dummy/SDL_dummy_main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/SDL2main.dir/src/main/dummy/SDL_dummy_main.c.i"
	cd /home/murii/Desktop/Programming/CLove/build/src/3rdparty/SDL2 && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/murii/Desktop/Programming/CLove/src/3rdparty/SDL2/src/main/dummy/SDL_dummy_main.c > CMakeFiles/SDL2main.dir/src/main/dummy/SDL_dummy_main.c.i

src/3rdparty/SDL2/CMakeFiles/SDL2main.dir/src/main/dummy/SDL_dummy_main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/SDL2main.dir/src/main/dummy/SDL_dummy_main.c.s"
	cd /home/murii/Desktop/Programming/CLove/build/src/3rdparty/SDL2 && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/murii/Desktop/Programming/CLove/src/3rdparty/SDL2/src/main/dummy/SDL_dummy_main.c -o CMakeFiles/SDL2main.dir/src/main/dummy/SDL_dummy_main.c.s

src/3rdparty/SDL2/CMakeFiles/SDL2main.dir/src/main/dummy/SDL_dummy_main.c.o.requires:

.PHONY : src/3rdparty/SDL2/CMakeFiles/SDL2main.dir/src/main/dummy/SDL_dummy_main.c.o.requires

src/3rdparty/SDL2/CMakeFiles/SDL2main.dir/src/main/dummy/SDL_dummy_main.c.o.provides: src/3rdparty/SDL2/CMakeFiles/SDL2main.dir/src/main/dummy/SDL_dummy_main.c.o.requires
	$(MAKE) -f src/3rdparty/SDL2/CMakeFiles/SDL2main.dir/build.make src/3rdparty/SDL2/CMakeFiles/SDL2main.dir/src/main/dummy/SDL_dummy_main.c.o.provides.build
.PHONY : src/3rdparty/SDL2/CMakeFiles/SDL2main.dir/src/main/dummy/SDL_dummy_main.c.o.provides

src/3rdparty/SDL2/CMakeFiles/SDL2main.dir/src/main/dummy/SDL_dummy_main.c.o.provides.build: src/3rdparty/SDL2/CMakeFiles/SDL2main.dir/src/main/dummy/SDL_dummy_main.c.o


# Object files for target SDL2main
SDL2main_OBJECTS = \
"CMakeFiles/SDL2main.dir/src/main/dummy/SDL_dummy_main.c.o"

# External object files for target SDL2main
SDL2main_EXTERNAL_OBJECTS =

src/3rdparty/SDL2/libSDL2main.a: src/3rdparty/SDL2/CMakeFiles/SDL2main.dir/src/main/dummy/SDL_dummy_main.c.o
src/3rdparty/SDL2/libSDL2main.a: src/3rdparty/SDL2/CMakeFiles/SDL2main.dir/build.make
src/3rdparty/SDL2/libSDL2main.a: src/3rdparty/SDL2/CMakeFiles/SDL2main.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/murii/Desktop/Programming/CLove/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C static library libSDL2main.a"
	cd /home/murii/Desktop/Programming/CLove/build/src/3rdparty/SDL2 && $(CMAKE_COMMAND) -P CMakeFiles/SDL2main.dir/cmake_clean_target.cmake
	cd /home/murii/Desktop/Programming/CLove/build/src/3rdparty/SDL2 && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/SDL2main.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/3rdparty/SDL2/CMakeFiles/SDL2main.dir/build: src/3rdparty/SDL2/libSDL2main.a

.PHONY : src/3rdparty/SDL2/CMakeFiles/SDL2main.dir/build

src/3rdparty/SDL2/CMakeFiles/SDL2main.dir/requires: src/3rdparty/SDL2/CMakeFiles/SDL2main.dir/src/main/dummy/SDL_dummy_main.c.o.requires

.PHONY : src/3rdparty/SDL2/CMakeFiles/SDL2main.dir/requires

src/3rdparty/SDL2/CMakeFiles/SDL2main.dir/clean:
	cd /home/murii/Desktop/Programming/CLove/build/src/3rdparty/SDL2 && $(CMAKE_COMMAND) -P CMakeFiles/SDL2main.dir/cmake_clean.cmake
.PHONY : src/3rdparty/SDL2/CMakeFiles/SDL2main.dir/clean

src/3rdparty/SDL2/CMakeFiles/SDL2main.dir/depend:
	cd /home/murii/Desktop/Programming/CLove/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/murii/Desktop/Programming/CLove /home/murii/Desktop/Programming/CLove/src/3rdparty/SDL2 /home/murii/Desktop/Programming/CLove/build /home/murii/Desktop/Programming/CLove/build/src/3rdparty/SDL2 /home/murii/Desktop/Programming/CLove/build/src/3rdparty/SDL2/CMakeFiles/SDL2main.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/3rdparty/SDL2/CMakeFiles/SDL2main.dir/depend

