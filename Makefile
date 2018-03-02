

# How to place object files in separate subdirectory
# https://stackoverflow.com/questions/5178125/how-to-place-object-files-in-separate-subdirectory


# Compiler and Linker
CC          := g++

# The Target Binary Program
TARGET      := main

# The Directories, Source, Includes, Objects, Binary and Resources
SRCDIR      := src
INCDIR      := inc
BUILDDIR    := objects
DEPDIR      := dependencies
TARGETDIR   := bin
RESDIR      := res
SRCEXT      := cpp
DEPEXT      := d
OBJEXT      := o

# Flags, Libraries and Includes
CFLAGS      := -Wall -O3 -g
LIB         := `pkg-config --cflags --libs gtk+-3.0 gtkmm-3.0 glibmm-2.4`
INC         := -I$(INCDIR) -I/usr/local/include
INCDEP      := -I$(INCDIR)



#---------------------------------------------------------------------------------
# DO NOT EDIT BELOW THIS LINE
#---------------------------------------------------------------------------------
SOURCES     := $(shell find $(SRCDIR) -type f -name "*.$(SRCEXT)")
OBJECTS     := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.$(OBJEXT)))


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
	@mkdir -p $(DEPDIR)
	@mkdir -p $(BUILDDIR)


# Clean only Objecst
clean:
	@$(RM) -rf $(BUILDDIR)


# Full Clean, Objects and Binaries
cleaner: veryclean
veryclean: cleaner
	@$(RM) -rf $(DEPDIR)
	@$(RM) -rf $(TARGETDIR)


# Pull in dependency info for *existing* .o files
-include $(OBJECTS:.$(OBJEXT)=.$(DEPEXT))


# Link
$(TARGET): $(OBJECTS)
	$(CC) -o $(TARGETDIR)/$(TARGET) $^ $(LIB)


# Compile
# @echo "Building target: $@ from $<"
$(BUILDDIR)/%.$(OBJEXT): $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INC) -c -o $@ $< $(LIB)
	@$(CC) $(CFLAGS) $(INCDEP) -MM $(SRCDIR)/$*.$(SRCEXT) > $(DEPDIR)/$*.$(DEPEXT)
	@cp -f $(DEPDIR)/$*.$(DEPEXT) $(DEPDIR)/$*.$(DEPEXT).tmp
	@sed -e 's|.*:|$(DEPDIR)/$*.$(OBJEXT):|' < $(DEPDIR)/$*.$(DEPEXT).tmp > $(DEPDIR)/$*.$(DEPEXT)
	@sed -e 's/.*://' -e 's/\\$$//' < $(DEPDIR)/$*.$(DEPEXT).tmp | fmt -1 | sed -e 's/^ *//' -e 's/$$/:/' >> $(DEPDIR)/$*.$(DEPEXT)
	@rm -f $(DEPDIR)/$*.$(DEPEXT).tmp


