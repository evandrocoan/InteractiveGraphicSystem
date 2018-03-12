

# How to place object files in separate subdirectory
# https://stackoverflow.com/questions/5178125/how-to-place-object-files-in-separate-subdirectory


# Compiler and Linker
CC := g++

# The Target Binary Program
TARGET := main

# The Directories, Source, Includes, Objects, Binary and Resources
SRCDIR:= src
RESDIR:= resources
SRCEXT:= cpp
OBJEXT:= o
DEPEXT:= d
BUILDDIR:= objects
TARGETDIR:= binaries

CFLAGS := -O0 -g -std=c++11
# CFLAGS := -Wall -O3 -g -std=c++11

LIBS := `pkg-config --cflags --libs glibmm-2.4 giomm-2.4 gtkmm-3.0`
FIND_EXCLUSIONS := \( ! -name "*test.cpp" \)

INC := -I.
# INC := -I. -I/usr/local/include
INCDEP := -I.


##
## Usage:
##   make <target> [options]
##
## Targets:
##   all               generate all assets
##   run               build and open the compiled program
##   clean             remove the objects and dependencies directory
##   veryclean         same as `clean`, but also removes the `bin` folder
##
## Options:
##   -p                show all commands run by make, including all variables
##   -n                similar to --dry-run, it will only display the commands,
##                     but not run them.
##
## For more explanations see:
##   http://www.oreilly.com/openbook/make3/book/ch12.pdf
##

#
# Font:
# https://tex.stackexchange.com/questions/40738/how-to-properly-make-a-latex-project
#
# You want latexmk to *always* run, because make does not have all the info.
# Also, include non-file targets in .PHONY so they are run regardless of any
# file of the given name existing.
.PHONY: start_timer print_elapsed_time



#---------------------------------------------------------------------------------
# DO NOT EDIT BELOW THIS LINE
#---------------------------------------------------------------------------------
SOURCES := $(shell /bin/find $(SRCDIR) -type f -name "*.$(SRCEXT)" $(FIND_EXCLUSIONS))
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.$(OBJEXT)))

CURRENT_DIR := $(shell pwd)

# Print the usage instructions
# https://gist.github.com/prwhite/8168133
help:
	@fgrep -h "##" $(MAKEFILE_LIST) | fgrep -v fgrep | sed -e 's/\\$$//' | sed -e 's/##//'

# Default make target rule
# How to force a certain groups of targets to be always run sequentially?
# https://stackoverflow.com/questions/21832023/how-to-force-a-certain-groups-of-targets-to-be-always-run-sequentially
all:
	@${MAKE} start_timer -s
	@${MAKE} resources -s
	${MAKE} $(TARGET) -j
	@${MAKE} print_elapsed_time -s

# GNU Make silent by default
# https://stackoverflow.com/questions/24005166/gnu-make-silent-by-default
.SILENT: start_timer print_elapsed_time

# Start counting the elapsed seconds to print them to the screen later
start_timer:
	. ./shell_scripts/timer_calculator.sh "$(CURRENT_DIR)"

# Calculate the elapsed seconds and print them to the screen
print_elapsed_time:
	. ./shell_scripts/timer_calculator.sh; showTheElapsedSeconds


run: all
	./$(TARGETDIR)/$(TARGET)

# Remake
remake: cleaner all

# Copy Resources from Resources Directory to Target Directory
resources: directories
	if [ -d $(RESDIR) ]; then cp $(RESDIR)/* $(TARGETDIR)/; fi;

# Make the Directories
directories:
	@mkdir -p $(TARGETDIR)
	@mkdir -p $(BUILDDIR)


# Clean only Objecst
clean:
	$(RM) -rfv $(BUILDDIR)

# Full Clean, Objects and Binaries
cleaner: veryclean
veryclean: clean
	$(RM) -rfv $(TARGETDIR)


# Pull in dependency info for *existing* .o files
-include $(OBJECTS:.$(OBJEXT)=.$(DEPEXT))

# Link
$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGETDIR)/$(TARGET) $^ $(LIBS)

# Compile
# @echo "Building target: $@ from $<"
#
# Autodependencies with GNU make, Scott McPeak, November 2001
# http://scottmcpeak.com/autodepend/autodepend.html
#
# Compile and generate dependency info;
# More complicated dependency computation, so all prereqs listed
# will also become command-less, prereq-less targets:
#   sed:    strip the target (everything before colon)
#   sed:    remove any continuation backslashes
#   fmt -1: list words one per line
#   sed:    strip leading spaces
#   sed:    add trailing colons
$(BUILDDIR)/%.$(OBJEXT): $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INC) -c -o $@ $< $(LIBS)
	@$(CC) $(CFLAGS) $(INCDEP) -MM $(SRCDIR)/$*.$(SRCEXT) > $(BUILDDIR)/$*.$(DEPEXT)
	@cp -f $(BUILDDIR)/$*.$(DEPEXT) $(BUILDDIR)/$*.$(DEPEXT).tmp
	@sed -e 's|.*:|$(BUILDDIR)/$*.$(OBJEXT):|' < $(BUILDDIR)/$*.$(DEPEXT).tmp > $(BUILDDIR)/$*.$(DEPEXT)
	@sed -e 's/.*://' -e 's/\\$$//' < $(BUILDDIR)/$*.$(DEPEXT).tmp | fmt -1 | sed -e 's/^ *//' -e 's/$$/:/' >> $(BUILDDIR)/$*.$(DEPEXT)
	@rm -f $(BUILDDIR)/$*.$(DEPEXT).tmp


