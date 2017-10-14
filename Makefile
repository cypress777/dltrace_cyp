# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.7

# Default target executed when no arguments are given to make.
default_target: all

.PHONY : default_target

# Allow only one "make -f Makefile2" at a time, but pass parallelism.
.NOTPARALLEL:


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
CMAKE_COMMAND = /usr/local/Cellar/cmake/3.7.1/bin/cmake

# The command to remove a file.
RM = /usr/local/Cellar/cmake/3.7.1/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/cypress/Works/dtrace-v1.0/dltrace

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/cypress/Works/dtrace-v1.0/dltrace

#=============================================================================
# Targets provided globally by CMake.

# Special rule for the target rebuild_cache
rebuild_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake to regenerate build system..."
	/usr/local/Cellar/cmake/3.7.1/bin/cmake -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : rebuild_cache

# Special rule for the target rebuild_cache
rebuild_cache/fast: rebuild_cache

.PHONY : rebuild_cache/fast

# Special rule for the target edit_cache
edit_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake cache editor..."
	/usr/local/Cellar/cmake/3.7.1/bin/ccmake -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : edit_cache

# Special rule for the target edit_cache
edit_cache/fast: edit_cache

.PHONY : edit_cache/fast

# The main all target
all: cmake_check_build_system
	$(CMAKE_COMMAND) -E cmake_progress_start /Users/cypress/Works/dtrace-v1.0/dltrace/CMakeFiles /Users/cypress/Works/dtrace-v1.0/dltrace/CMakeFiles/progress.marks
	$(MAKE) -f CMakeFiles/Makefile2 all
	$(CMAKE_COMMAND) -E cmake_progress_start /Users/cypress/Works/dtrace-v1.0/dltrace/CMakeFiles 0
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
# Target rules for targets named dltrace

# Build rule for target.
dltrace: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 dltrace
.PHONY : dltrace

# fast build rule for target.
dltrace/fast:
	$(MAKE) -f CMakeFiles/dltrace.dir/build.make CMakeFiles/dltrace.dir/build
.PHONY : dltrace/fast

event/breakpointevent.o: event/breakpointevent.cpp.o

.PHONY : event/breakpointevent.o

# target to build an object file
event/breakpointevent.cpp.o:
	$(MAKE) -f CMakeFiles/dltrace.dir/build.make CMakeFiles/dltrace.dir/event/breakpointevent.cpp.o
.PHONY : event/breakpointevent.cpp.o

event/breakpointevent.i: event/breakpointevent.cpp.i

.PHONY : event/breakpointevent.i

# target to preprocess a source file
event/breakpointevent.cpp.i:
	$(MAKE) -f CMakeFiles/dltrace.dir/build.make CMakeFiles/dltrace.dir/event/breakpointevent.cpp.i
.PHONY : event/breakpointevent.cpp.i

event/breakpointevent.s: event/breakpointevent.cpp.s

.PHONY : event/breakpointevent.s

# target to generate assembly for a file
event/breakpointevent.cpp.s:
	$(MAKE) -f CMakeFiles/dltrace.dir/build.make CMakeFiles/dltrace.dir/event/breakpointevent.cpp.s
.PHONY : event/breakpointevent.cpp.s

event/cloneevent.o: event/cloneevent.cpp.o

.PHONY : event/cloneevent.o

# target to build an object file
event/cloneevent.cpp.o:
	$(MAKE) -f CMakeFiles/dltrace.dir/build.make CMakeFiles/dltrace.dir/event/cloneevent.cpp.o
.PHONY : event/cloneevent.cpp.o

event/cloneevent.i: event/cloneevent.cpp.i

.PHONY : event/cloneevent.i

# target to preprocess a source file
event/cloneevent.cpp.i:
	$(MAKE) -f CMakeFiles/dltrace.dir/build.make CMakeFiles/dltrace.dir/event/cloneevent.cpp.i
.PHONY : event/cloneevent.cpp.i

event/cloneevent.s: event/cloneevent.cpp.s

.PHONY : event/cloneevent.s

# target to generate assembly for a file
event/cloneevent.cpp.s:
	$(MAKE) -f CMakeFiles/dltrace.dir/build.make CMakeFiles/dltrace.dir/event/cloneevent.cpp.s
.PHONY : event/cloneevent.cpp.s

event/event.o: event/event.cpp.o

.PHONY : event/event.o

# target to build an object file
event/event.cpp.o:
	$(MAKE) -f CMakeFiles/dltrace.dir/build.make CMakeFiles/dltrace.dir/event/event.cpp.o
.PHONY : event/event.cpp.o

event/event.i: event/event.cpp.i

.PHONY : event/event.i

# target to preprocess a source file
event/event.cpp.i:
	$(MAKE) -f CMakeFiles/dltrace.dir/build.make CMakeFiles/dltrace.dir/event/event.cpp.i
.PHONY : event/event.cpp.i

event/event.s: event/event.cpp.s

.PHONY : event/event.s

# target to generate assembly for a file
event/event.cpp.s:
	$(MAKE) -f CMakeFiles/dltrace.dir/build.make CMakeFiles/dltrace.dir/event/event.cpp.s
.PHONY : event/event.cpp.s

event/eventhandler.o: event/eventhandler.cpp.o

.PHONY : event/eventhandler.o

# target to build an object file
event/eventhandler.cpp.o:
	$(MAKE) -f CMakeFiles/dltrace.dir/build.make CMakeFiles/dltrace.dir/event/eventhandler.cpp.o
.PHONY : event/eventhandler.cpp.o

event/eventhandler.i: event/eventhandler.cpp.i

.PHONY : event/eventhandler.i

# target to preprocess a source file
event/eventhandler.cpp.i:
	$(MAKE) -f CMakeFiles/dltrace.dir/build.make CMakeFiles/dltrace.dir/event/eventhandler.cpp.i
.PHONY : event/eventhandler.cpp.i

event/eventhandler.s: event/eventhandler.cpp.s

.PHONY : event/eventhandler.s

# target to generate assembly for a file
event/eventhandler.cpp.s:
	$(MAKE) -f CMakeFiles/dltrace.dir/build.make CMakeFiles/dltrace.dir/event/eventhandler.cpp.s
.PHONY : event/eventhandler.cpp.s

event/exitedevent.o: event/exitedevent.cpp.o

.PHONY : event/exitedevent.o

# target to build an object file
event/exitedevent.cpp.o:
	$(MAKE) -f CMakeFiles/dltrace.dir/build.make CMakeFiles/dltrace.dir/event/exitedevent.cpp.o
.PHONY : event/exitedevent.cpp.o

event/exitedevent.i: event/exitedevent.cpp.i

.PHONY : event/exitedevent.i

# target to preprocess a source file
event/exitedevent.cpp.i:
	$(MAKE) -f CMakeFiles/dltrace.dir/build.make CMakeFiles/dltrace.dir/event/exitedevent.cpp.i
.PHONY : event/exitedevent.cpp.i

event/exitedevent.s: event/exitedevent.cpp.s

.PHONY : event/exitedevent.s

# target to generate assembly for a file
event/exitedevent.cpp.s:
	$(MAKE) -f CMakeFiles/dltrace.dir/build.make CMakeFiles/dltrace.dir/event/exitedevent.cpp.s
.PHONY : event/exitedevent.cpp.s

event/ignoreevent.o: event/ignoreevent.cpp.o

.PHONY : event/ignoreevent.o

# target to build an object file
event/ignoreevent.cpp.o:
	$(MAKE) -f CMakeFiles/dltrace.dir/build.make CMakeFiles/dltrace.dir/event/ignoreevent.cpp.o
.PHONY : event/ignoreevent.cpp.o

event/ignoreevent.i: event/ignoreevent.cpp.i

.PHONY : event/ignoreevent.i

# target to preprocess a source file
event/ignoreevent.cpp.i:
	$(MAKE) -f CMakeFiles/dltrace.dir/build.make CMakeFiles/dltrace.dir/event/ignoreevent.cpp.i
.PHONY : event/ignoreevent.cpp.i

event/ignoreevent.s: event/ignoreevent.cpp.s

.PHONY : event/ignoreevent.s

# target to generate assembly for a file
event/ignoreevent.cpp.s:
	$(MAKE) -f CMakeFiles/dltrace.dir/build.make CMakeFiles/dltrace.dir/event/ignoreevent.cpp.s
.PHONY : event/ignoreevent.cpp.s

event/newprocessevent.o: event/newprocessevent.cpp.o

.PHONY : event/newprocessevent.o

# target to build an object file
event/newprocessevent.cpp.o:
	$(MAKE) -f CMakeFiles/dltrace.dir/build.make CMakeFiles/dltrace.dir/event/newprocessevent.cpp.o
.PHONY : event/newprocessevent.cpp.o

event/newprocessevent.i: event/newprocessevent.cpp.i

.PHONY : event/newprocessevent.i

# target to preprocess a source file
event/newprocessevent.cpp.i:
	$(MAKE) -f CMakeFiles/dltrace.dir/build.make CMakeFiles/dltrace.dir/event/newprocessevent.cpp.i
.PHONY : event/newprocessevent.cpp.i

event/newprocessevent.s: event/newprocessevent.cpp.s

.PHONY : event/newprocessevent.s

# target to generate assembly for a file
event/newprocessevent.cpp.s:
	$(MAKE) -f CMakeFiles/dltrace.dir/build.make CMakeFiles/dltrace.dir/event/newprocessevent.cpp.s
.PHONY : event/newprocessevent.cpp.s

event/signaledevent.o: event/signaledevent.cpp.o

.PHONY : event/signaledevent.o

# target to build an object file
event/signaledevent.cpp.o:
	$(MAKE) -f CMakeFiles/dltrace.dir/build.make CMakeFiles/dltrace.dir/event/signaledevent.cpp.o
.PHONY : event/signaledevent.cpp.o

event/signaledevent.i: event/signaledevent.cpp.i

.PHONY : event/signaledevent.i

# target to preprocess a source file
event/signaledevent.cpp.i:
	$(MAKE) -f CMakeFiles/dltrace.dir/build.make CMakeFiles/dltrace.dir/event/signaledevent.cpp.i
.PHONY : event/signaledevent.cpp.i

event/signaledevent.s: event/signaledevent.cpp.s

.PHONY : event/signaledevent.s

# target to generate assembly for a file
event/signaledevent.cpp.s:
	$(MAKE) -f CMakeFiles/dltrace.dir/build.make CMakeFiles/dltrace.dir/event/signaledevent.cpp.s
.PHONY : event/signaledevent.cpp.s

event/signalexitedevent.o: event/signalexitedevent.cpp.o

.PHONY : event/signalexitedevent.o

# target to build an object file
event/signalexitedevent.cpp.o:
	$(MAKE) -f CMakeFiles/dltrace.dir/build.make CMakeFiles/dltrace.dir/event/signalexitedevent.cpp.o
.PHONY : event/signalexitedevent.cpp.o

event/signalexitedevent.i: event/signalexitedevent.cpp.i

.PHONY : event/signalexitedevent.i

# target to preprocess a source file
event/signalexitedevent.cpp.i:
	$(MAKE) -f CMakeFiles/dltrace.dir/build.make CMakeFiles/dltrace.dir/event/signalexitedevent.cpp.i
.PHONY : event/signalexitedevent.cpp.i

event/signalexitedevent.s: event/signalexitedevent.cpp.s

.PHONY : event/signalexitedevent.s

# target to generate assembly for a file
event/signalexitedevent.cpp.s:
	$(MAKE) -f CMakeFiles/dltrace.dir/build.make CMakeFiles/dltrace.dir/event/signalexitedevent.cpp.s
.PHONY : event/signalexitedevent.cpp.s

event/singlesteppedevent.o: event/singlesteppedevent.cpp.o

.PHONY : event/singlesteppedevent.o

# target to build an object file
event/singlesteppedevent.cpp.o:
	$(MAKE) -f CMakeFiles/dltrace.dir/build.make CMakeFiles/dltrace.dir/event/singlesteppedevent.cpp.o
.PHONY : event/singlesteppedevent.cpp.o

event/singlesteppedevent.i: event/singlesteppedevent.cpp.i

.PHONY : event/singlesteppedevent.i

# target to preprocess a source file
event/singlesteppedevent.cpp.i:
	$(MAKE) -f CMakeFiles/dltrace.dir/build.make CMakeFiles/dltrace.dir/event/singlesteppedevent.cpp.i
.PHONY : event/singlesteppedevent.cpp.i

event/singlesteppedevent.s: event/singlesteppedevent.cpp.s

.PHONY : event/singlesteppedevent.s

# target to generate assembly for a file
event/singlesteppedevent.cpp.s:
	$(MAKE) -f CMakeFiles/dltrace.dir/build.make CMakeFiles/dltrace.dir/event/singlesteppedevent.cpp.s
.PHONY : event/singlesteppedevent.cpp.s

main.o: main.cpp.o

.PHONY : main.o

# target to build an object file
main.cpp.o:
	$(MAKE) -f CMakeFiles/dltrace.dir/build.make CMakeFiles/dltrace.dir/main.cpp.o
.PHONY : main.cpp.o

main.i: main.cpp.i

.PHONY : main.i

# target to preprocess a source file
main.cpp.i:
	$(MAKE) -f CMakeFiles/dltrace.dir/build.make CMakeFiles/dltrace.dir/main.cpp.i
.PHONY : main.cpp.i

main.s: main.cpp.s

.PHONY : main.s

# target to generate assembly for a file
main.cpp.s:
	$(MAKE) -f CMakeFiles/dltrace.dir/build.make CMakeFiles/dltrace.dir/main.cpp.s
.PHONY : main.cpp.s

record.o: record.cpp.o

.PHONY : record.o

# target to build an object file
record.cpp.o:
	$(MAKE) -f CMakeFiles/dltrace.dir/build.make CMakeFiles/dltrace.dir/record.cpp.o
.PHONY : record.cpp.o

record.i: record.cpp.i

.PHONY : record.i

# target to preprocess a source file
record.cpp.i:
	$(MAKE) -f CMakeFiles/dltrace.dir/build.make CMakeFiles/dltrace.dir/record.cpp.i
.PHONY : record.cpp.i

record.s: record.cpp.s

.PHONY : record.s

# target to generate assembly for a file
record.cpp.s:
	$(MAKE) -f CMakeFiles/dltrace.dir/build.make CMakeFiles/dltrace.dir/record.cpp.s
.PHONY : record.cpp.s

utils/breakpoint.o: utils/breakpoint.cpp.o

.PHONY : utils/breakpoint.o

# target to build an object file
utils/breakpoint.cpp.o:
	$(MAKE) -f CMakeFiles/dltrace.dir/build.make CMakeFiles/dltrace.dir/utils/breakpoint.cpp.o
.PHONY : utils/breakpoint.cpp.o

utils/breakpoint.i: utils/breakpoint.cpp.i

.PHONY : utils/breakpoint.i

# target to preprocess a source file
utils/breakpoint.cpp.i:
	$(MAKE) -f CMakeFiles/dltrace.dir/build.make CMakeFiles/dltrace.dir/utils/breakpoint.cpp.i
.PHONY : utils/breakpoint.cpp.i

utils/breakpoint.s: utils/breakpoint.cpp.s

.PHONY : utils/breakpoint.s

# target to generate assembly for a file
utils/breakpoint.cpp.s:
	$(MAKE) -f CMakeFiles/dltrace.dir/build.make CMakeFiles/dltrace.dir/utils/breakpoint.cpp.s
.PHONY : utils/breakpoint.cpp.s

utils/demangle.o: utils/demangle.cpp.o

.PHONY : utils/demangle.o

# target to build an object file
utils/demangle.cpp.o:
	$(MAKE) -f CMakeFiles/dltrace.dir/build.make CMakeFiles/dltrace.dir/utils/demangle.cpp.o
.PHONY : utils/demangle.cpp.o

utils/demangle.i: utils/demangle.cpp.i

.PHONY : utils/demangle.i

# target to preprocess a source file
utils/demangle.cpp.i:
	$(MAKE) -f CMakeFiles/dltrace.dir/build.make CMakeFiles/dltrace.dir/utils/demangle.cpp.i
.PHONY : utils/demangle.cpp.i

utils/demangle.s: utils/demangle.cpp.s

.PHONY : utils/demangle.s

# target to generate assembly for a file
utils/demangle.cpp.s:
	$(MAKE) -f CMakeFiles/dltrace.dir/build.make CMakeFiles/dltrace.dir/utils/demangle.cpp.s
.PHONY : utils/demangle.cpp.s

utils/funcmsg.o: utils/funcmsg.cpp.o

.PHONY : utils/funcmsg.o

# target to build an object file
utils/funcmsg.cpp.o:
	$(MAKE) -f CMakeFiles/dltrace.dir/build.make CMakeFiles/dltrace.dir/utils/funcmsg.cpp.o
.PHONY : utils/funcmsg.cpp.o

utils/funcmsg.i: utils/funcmsg.cpp.i

.PHONY : utils/funcmsg.i

# target to preprocess a source file
utils/funcmsg.cpp.i:
	$(MAKE) -f CMakeFiles/dltrace.dir/build.make CMakeFiles/dltrace.dir/utils/funcmsg.cpp.i
.PHONY : utils/funcmsg.cpp.i

utils/funcmsg.s: utils/funcmsg.cpp.s

.PHONY : utils/funcmsg.s

# target to generate assembly for a file
utils/funcmsg.cpp.s:
	$(MAKE) -f CMakeFiles/dltrace.dir/build.make CMakeFiles/dltrace.dir/utils/funcmsg.cpp.s
.PHONY : utils/funcmsg.cpp.s

utils/funcmsgsutil.o: utils/funcmsgsutil.cpp.o

.PHONY : utils/funcmsgsutil.o

# target to build an object file
utils/funcmsgsutil.cpp.o:
	$(MAKE) -f CMakeFiles/dltrace.dir/build.make CMakeFiles/dltrace.dir/utils/funcmsgsutil.cpp.o
.PHONY : utils/funcmsgsutil.cpp.o

utils/funcmsgsutil.i: utils/funcmsgsutil.cpp.i

.PHONY : utils/funcmsgsutil.i

# target to preprocess a source file
utils/funcmsgsutil.cpp.i:
	$(MAKE) -f CMakeFiles/dltrace.dir/build.make CMakeFiles/dltrace.dir/utils/funcmsgsutil.cpp.i
.PHONY : utils/funcmsgsutil.cpp.i

utils/funcmsgsutil.s: utils/funcmsgsutil.cpp.s

.PHONY : utils/funcmsgsutil.s

# target to generate assembly for a file
utils/funcmsgsutil.cpp.s:
	$(MAKE) -f CMakeFiles/dltrace.dir/build.make CMakeFiles/dltrace.dir/utils/funcmsgsutil.cpp.s
.PHONY : utils/funcmsgsutil.cpp.s

utils/options.o: utils/options.cpp.o

.PHONY : utils/options.o

# target to build an object file
utils/options.cpp.o:
	$(MAKE) -f CMakeFiles/dltrace.dir/build.make CMakeFiles/dltrace.dir/utils/options.cpp.o
.PHONY : utils/options.cpp.o

utils/options.i: utils/options.cpp.i

.PHONY : utils/options.i

# target to preprocess a source file
utils/options.cpp.i:
	$(MAKE) -f CMakeFiles/dltrace.dir/build.make CMakeFiles/dltrace.dir/utils/options.cpp.i
.PHONY : utils/options.cpp.i

utils/options.s: utils/options.cpp.s

.PHONY : utils/options.s

# target to generate assembly for a file
utils/options.cpp.s:
	$(MAKE) -f CMakeFiles/dltrace.dir/build.make CMakeFiles/dltrace.dir/utils/options.cpp.s
.PHONY : utils/options.cpp.s

utils/process.o: utils/process.cpp.o

.PHONY : utils/process.o

# target to build an object file
utils/process.cpp.o:
	$(MAKE) -f CMakeFiles/dltrace.dir/build.make CMakeFiles/dltrace.dir/utils/process.cpp.o
.PHONY : utils/process.cpp.o

utils/process.i: utils/process.cpp.i

.PHONY : utils/process.i

# target to preprocess a source file
utils/process.cpp.i:
	$(MAKE) -f CMakeFiles/dltrace.dir/build.make CMakeFiles/dltrace.dir/utils/process.cpp.i
.PHONY : utils/process.cpp.i

utils/process.s: utils/process.cpp.s

.PHONY : utils/process.s

# target to generate assembly for a file
utils/process.cpp.s:
	$(MAKE) -f CMakeFiles/dltrace.dir/build.make CMakeFiles/dltrace.dir/utils/process.cpp.s
.PHONY : utils/process.cpp.s

utils/splitor.o: utils/splitor.cpp.o

.PHONY : utils/splitor.o

# target to build an object file
utils/splitor.cpp.o:
	$(MAKE) -f CMakeFiles/dltrace.dir/build.make CMakeFiles/dltrace.dir/utils/splitor.cpp.o
.PHONY : utils/splitor.cpp.o

utils/splitor.i: utils/splitor.cpp.i

.PHONY : utils/splitor.i

# target to preprocess a source file
utils/splitor.cpp.i:
	$(MAKE) -f CMakeFiles/dltrace.dir/build.make CMakeFiles/dltrace.dir/utils/splitor.cpp.i
.PHONY : utils/splitor.cpp.i

utils/splitor.s: utils/splitor.cpp.s

.PHONY : utils/splitor.s

# target to generate assembly for a file
utils/splitor.cpp.s:
	$(MAKE) -f CMakeFiles/dltrace.dir/build.make CMakeFiles/dltrace.dir/utils/splitor.cpp.s
.PHONY : utils/splitor.cpp.s

utils/timeex.o: utils/timeex.cpp.o

.PHONY : utils/timeex.o

# target to build an object file
utils/timeex.cpp.o:
	$(MAKE) -f CMakeFiles/dltrace.dir/build.make CMakeFiles/dltrace.dir/utils/timeex.cpp.o
.PHONY : utils/timeex.cpp.o

utils/timeex.i: utils/timeex.cpp.i

.PHONY : utils/timeex.i

# target to preprocess a source file
utils/timeex.cpp.i:
	$(MAKE) -f CMakeFiles/dltrace.dir/build.make CMakeFiles/dltrace.dir/utils/timeex.cpp.i
.PHONY : utils/timeex.cpp.i

utils/timeex.s: utils/timeex.cpp.s

.PHONY : utils/timeex.s

# target to generate assembly for a file
utils/timeex.cpp.s:
	$(MAKE) -f CMakeFiles/dltrace.dir/build.make CMakeFiles/dltrace.dir/utils/timeex.cpp.s
.PHONY : utils/timeex.cpp.s

utils/trace.o: utils/trace.cpp.o

.PHONY : utils/trace.o

# target to build an object file
utils/trace.cpp.o:
	$(MAKE) -f CMakeFiles/dltrace.dir/build.make CMakeFiles/dltrace.dir/utils/trace.cpp.o
.PHONY : utils/trace.cpp.o

utils/trace.i: utils/trace.cpp.i

.PHONY : utils/trace.i

# target to preprocess a source file
utils/trace.cpp.i:
	$(MAKE) -f CMakeFiles/dltrace.dir/build.make CMakeFiles/dltrace.dir/utils/trace.cpp.i
.PHONY : utils/trace.cpp.i

utils/trace.s: utils/trace.cpp.s

.PHONY : utils/trace.s

# target to generate assembly for a file
utils/trace.cpp.s:
	$(MAKE) -f CMakeFiles/dltrace.dir/build.make CMakeFiles/dltrace.dir/utils/trace.cpp.s
.PHONY : utils/trace.cpp.s

# Help Target
help:
	@echo "The following are some of the valid targets for this Makefile:"
	@echo "... all (the default if no target is provided)"
	@echo "... clean"
	@echo "... depend"
	@echo "... rebuild_cache"
	@echo "... edit_cache"
	@echo "... dltrace"
	@echo "... event/breakpointevent.o"
	@echo "... event/breakpointevent.i"
	@echo "... event/breakpointevent.s"
	@echo "... event/cloneevent.o"
	@echo "... event/cloneevent.i"
	@echo "... event/cloneevent.s"
	@echo "... event/event.o"
	@echo "... event/event.i"
	@echo "... event/event.s"
	@echo "... event/eventhandler.o"
	@echo "... event/eventhandler.i"
	@echo "... event/eventhandler.s"
	@echo "... event/exitedevent.o"
	@echo "... event/exitedevent.i"
	@echo "... event/exitedevent.s"
	@echo "... event/ignoreevent.o"
	@echo "... event/ignoreevent.i"
	@echo "... event/ignoreevent.s"
	@echo "... event/newprocessevent.o"
	@echo "... event/newprocessevent.i"
	@echo "... event/newprocessevent.s"
	@echo "... event/signaledevent.o"
	@echo "... event/signaledevent.i"
	@echo "... event/signaledevent.s"
	@echo "... event/signalexitedevent.o"
	@echo "... event/signalexitedevent.i"
	@echo "... event/signalexitedevent.s"
	@echo "... event/singlesteppedevent.o"
	@echo "... event/singlesteppedevent.i"
	@echo "... event/singlesteppedevent.s"
	@echo "... main.o"
	@echo "... main.i"
	@echo "... main.s"
	@echo "... record.o"
	@echo "... record.i"
	@echo "... record.s"
	@echo "... utils/breakpoint.o"
	@echo "... utils/breakpoint.i"
	@echo "... utils/breakpoint.s"
	@echo "... utils/demangle.o"
	@echo "... utils/demangle.i"
	@echo "... utils/demangle.s"
	@echo "... utils/funcmsg.o"
	@echo "... utils/funcmsg.i"
	@echo "... utils/funcmsg.s"
	@echo "... utils/funcmsgsutil.o"
	@echo "... utils/funcmsgsutil.i"
	@echo "... utils/funcmsgsutil.s"
	@echo "... utils/options.o"
	@echo "... utils/options.i"
	@echo "... utils/options.s"
	@echo "... utils/process.o"
	@echo "... utils/process.i"
	@echo "... utils/process.s"
	@echo "... utils/splitor.o"
	@echo "... utils/splitor.i"
	@echo "... utils/splitor.s"
	@echo "... utils/timeex.o"
	@echo "... utils/timeex.i"
	@echo "... utils/timeex.s"
	@echo "... utils/trace.o"
	@echo "... utils/trace.i"
	@echo "... utils/trace.s"
.PHONY : help



#=============================================================================
# Special targets to cleanup operation of make.

# Special rule to run CMake to check the build system integrity.
# No rule that depends on this can have commands that come from listfiles
# because they might be regenerated.
cmake_check_build_system:
	$(CMAKE_COMMAND) -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 0
.PHONY : cmake_check_build_system

