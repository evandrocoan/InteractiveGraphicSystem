

# How to place object files in separate subdirectory
# https://stackoverflow.com/questions/5178125/how-to-place-object-files-in-separate-subdirectory


# Compiler and Linker
CC          := g++

# The Target Binary Program
TARGET      := main

# The Directories, Source, Includes, Objects, Binary and Resources
SRCDIR      := src
INCDIR      := includes
BUILDDIR    := objects
TARGETDIR   := binaries
RESDIR      := resources
SRCEXT      := cpp
DEPEXT      := d
OBJEXT      := o

# Flags, Libraries and Includes
# GTK_LIBS    := `pkg-config --libs gtkmm-3.0`
# GTK_FLAGS   := `pkg-config --cflags gtkmm-3.0`
# LIBS        := $(GTK_LIBS)
LIBS        := `pkg-config --cflags --libs glibmm-2.4 giomm-2.4 gtkmm-3.0`
INC         := -I$(INCDIR) -I/usr/local/include
INCDEP      := -I$(INCDIR)
CFLAGS      := -O0 -g $(GTK_FLAGS)
# CFLAGS      := -Wall -O3 -g $(GTK_FLAGS)


##
## Usage:
##   make <target> [options]
##
## Targets:
##   all               generate all assets
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


#---------------------------------------------------------------------------------
# DO NOT EDIT BELOW THIS LINE
#---------------------------------------------------------------------------------
SOURCES     := $(shell find $(SRCDIR) -type f -name "*.$(SRCEXT)")
OBJECTS     := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.$(OBJEXT)))


# Print the usage instructions
# https://gist.github.com/prwhite/8168133
help:
	@fgrep -h "##" $(MAKEFILE_LIST) | fgrep -v fgrep | sed -e 's/\\$$//' | sed -e 's/##//'


# Default Make
all: resources $(TARGET)


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
	@$(RM) -rf $(BUILDDIR)


# Full Clean, Objects and Binaries
cleaner: veryclean
veryclean: cleaner
	@$(RM) -rf $(TARGETDIR)


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


