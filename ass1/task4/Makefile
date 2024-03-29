# Define the compiler and the linker. The linker must be defined since
# the implicit rule for linking uses CC as the linker. g++ can be
# changed to clang++.
CXX = g++
CC  = g++

# Define preprocessor, compiler, and linker flags. Uncomment the # lines
# if you use clang++ and wish to use libc++ instead of libstd++.
CPPFLAGS  = -std=c++11 -Iinclude/
CXXFLAGS  = -g -O2 -Wall -W -pedantic-errors
CXXFLAGS += -Wmissing-braces -Wparentheses
CXXFLAGS += -std=c++11
LDFLAGS   = -g
#CPPFLAGS += -stdlib=libc++
#CXXFLAGS += -stdlib=libc++
#LDFLAGS +=  -stdlib=libc++

# Libraries
#IS_ROOT := $(shell command -v root 2> /dev/null)
#root_config := `root-config`
#root_config := `jjj`
#if type root-config > /dev/null;
#ifdef IS_ROOT
#ifneq ($(root_config),"`root-config`")
#ifdef IS_ (,$(findstring root-config,$(root_config)))
ifdef ROOTSYS
	CPPFLAGS += `root-config --cflags` -D ROOTYES=1
	LDLIBS = `root-config --libs --glibs`
endif

# Targets
PROGS = main

all: $(PROGS)

# Targets rely on implicit rules for compiling and linking, the following is executed!
# $(CXX) $(CPPFLAGS) $(CXXFLAGS) -c -o $@ $<
# $(CC) $(LDFLAGS) $^ $(LOADLIBES) $(LDLIBS) -o $@
# $@ = the file name of the target of the rule
# $< = The name of the first prerequisite. If the target got its recipe from an implicit rule, this will be the first prerequisite added by the implicit rule.
# $^ = The names of all the prerequisites, with spaces between them.

main: main.o eventandstate.o

# Phony targets
.PHONY: all clean

# Standard clean
clean:
	rm -f *.o $(PROGS) deps/*.d

# Generate dependencies in *.d files
# The line starting with an @ indicates print-suppression. Lines ending with a backslash indicate line splitting.
deps/%.d: %.cc | deps
	@set -e; rm -f $@; \
	 $(CPP) -MM $(CPPFLAGS) $< > $@.$$$$; \
         sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
         rm -f $@.$$$$

deps:
	mkdir -p deps;

# Include the *.d files
SRC = $(wildcard *.cc)
SRC := $(addprefix deps/, $(SRC))
#keyword include calls the "created" makefiles and runs them
include $(SRC:.cc=.d)

#Target which allows you to print variables as "make print-VARIABLE"
print-%  : ; @echo $* = $($*)

