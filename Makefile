# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

# Default target executed when no arguments are given to make.
default_target: all
.PHONY : default_target

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

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/ghost/source/glimpse

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/ghost/source/glimpse

#=============================================================================
# Targets provided globally by CMake.

# Special rule for the target edit_cache
edit_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running interactive CMake command-line interface..."
	/usr/bin/cmake -i .
.PHONY : edit_cache

# Special rule for the target edit_cache
edit_cache/fast: edit_cache
.PHONY : edit_cache/fast

# Special rule for the target rebuild_cache
rebuild_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake to regenerate build system..."
	/usr/bin/cmake -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : rebuild_cache

# Special rule for the target rebuild_cache
rebuild_cache/fast: rebuild_cache
.PHONY : rebuild_cache/fast

# The main all target
all: cmake_check_build_system
	$(CMAKE_COMMAND) -E cmake_progress_start /home/ghost/source/glimpse/CMakeFiles /home/ghost/source/glimpse/CMakeFiles/progress.marks
	$(MAKE) -f CMakeFiles/Makefile2 all
	$(CMAKE_COMMAND) -E cmake_progress_start /home/ghost/source/glimpse/CMakeFiles 0
.PHONY : all

# The main clean target
clean:
	$(MAKE) -f CMakeFiles/Makefile2 clean
.PHONY : clean

# The main clean target
clean/fast: clean
.PHONY : clean/fast

# Prepare targets for installation.
preinstall: all
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall

# Prepare targets for installation.
preinstall/fast:
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall/fast

# clear depends
depend:
	$(CMAKE_COMMAND) -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 1
.PHONY : depend

#=============================================================================
# Target rules for targets named glimpse

# Build rule for target.
glimpse: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 glimpse
.PHONY : glimpse

# fast build rule for target.
glimpse/fast:
	$(MAKE) -f CMakeFiles/glimpse.dir/build.make CMakeFiles/glimpse.dir/build
.PHONY : glimpse/fast

#=============================================================================
# Target rules for targets named test/test_log

# Build rule for target.
test/test_log: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 test/test_log
.PHONY : test/test_log

# fast build rule for target.
test/test_log/fast:
	$(MAKE) -f CMakeFiles/test/test_log.dir/build.make CMakeFiles/test/test_log.dir/build
.PHONY : test/test_log/fast

src/log.o: src/log.c.o
.PHONY : src/log.o

# target to build an object file
src/log.c.o:
	$(MAKE) -f CMakeFiles/glimpse.dir/build.make CMakeFiles/glimpse.dir/src/log.c.o
.PHONY : src/log.c.o

src/log.i: src/log.c.i
.PHONY : src/log.i

# target to preprocess a source file
src/log.c.i:
	$(MAKE) -f CMakeFiles/glimpse.dir/build.make CMakeFiles/glimpse.dir/src/log.c.i
.PHONY : src/log.c.i

src/log.s: src/log.c.s
.PHONY : src/log.s

# target to generate assembly for a file
src/log.c.s:
	$(MAKE) -f CMakeFiles/glimpse.dir/build.make CMakeFiles/glimpse.dir/src/log.c.s
.PHONY : src/log.c.s

src/pluginloader.o: src/pluginloader.c.o
.PHONY : src/pluginloader.o

# target to build an object file
src/pluginloader.c.o:
	$(MAKE) -f CMakeFiles/glimpse.dir/build.make CMakeFiles/glimpse.dir/src/pluginloader.c.o
.PHONY : src/pluginloader.c.o

src/pluginloader.i: src/pluginloader.c.i
.PHONY : src/pluginloader.i

# target to preprocess a source file
src/pluginloader.c.i:
	$(MAKE) -f CMakeFiles/glimpse.dir/build.make CMakeFiles/glimpse.dir/src/pluginloader.c.i
.PHONY : src/pluginloader.c.i

src/pluginloader.s: src/pluginloader.c.s
.PHONY : src/pluginloader.s

# target to generate assembly for a file
src/pluginloader.c.s:
	$(MAKE) -f CMakeFiles/glimpse.dir/build.make CMakeFiles/glimpse.dir/src/pluginloader.c.s
.PHONY : src/pluginloader.c.s

test/log.o: test/log.c.o
.PHONY : test/log.o

# target to build an object file
test/log.c.o:
	$(MAKE) -f CMakeFiles/test/test_log.dir/build.make CMakeFiles/test/test_log.dir/test/log.c.o
.PHONY : test/log.c.o

test/log.i: test/log.c.i
.PHONY : test/log.i

# target to preprocess a source file
test/log.c.i:
	$(MAKE) -f CMakeFiles/test/test_log.dir/build.make CMakeFiles/test/test_log.dir/test/log.c.i
.PHONY : test/log.c.i

test/log.s: test/log.c.s
.PHONY : test/log.s

# target to generate assembly for a file
test/log.c.s:
	$(MAKE) -f CMakeFiles/test/test_log.dir/build.make CMakeFiles/test/test_log.dir/test/log.c.s
.PHONY : test/log.c.s

# Help Target
help:
	@echo "The following are some of the valid targets for this Makefile:"
	@echo "... all (the default if no target is provided)"
	@echo "... clean"
	@echo "... depend"
	@echo "... edit_cache"
	@echo "... glimpse"
	@echo "... rebuild_cache"
	@echo "... test/test_log"
	@echo "... src/log.o"
	@echo "... src/log.i"
	@echo "... src/log.s"
	@echo "... src/pluginloader.o"
	@echo "... src/pluginloader.i"
	@echo "... src/pluginloader.s"
	@echo "... test/log.o"
	@echo "... test/log.i"
	@echo "... test/log.s"
.PHONY : help



#=============================================================================
# Special targets to cleanup operation of make.

# Special rule to run CMake to check the build system integrity.
# No rule that depends on this can have commands that come from listfiles
# because they might be regenerated.
cmake_check_build_system:
	$(CMAKE_COMMAND) -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 0
.PHONY : cmake_check_build_system

