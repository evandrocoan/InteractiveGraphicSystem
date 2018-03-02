

# How to place object files in separate subdirectory
# https://stackoverflow.com/questions/5178125/how-to-place-object-files-in-separate-subdirectory


#Compiler and Linker
CC          := g++

#The Target Binary Program
TARGET      := main

#The Directories, Source, Includes, Objects, Binary and Resources
SRCDIR      := src
INCDIR      := inc
BUILDDIR    := obj
TARGETDIR   := bin
RESDIR      := res
SRCEXT      := cpp
DEPEXT      := d
OBJEXT      := o

#Flags, Libraries and Includes
CFLAGS      := -Wall -O3 -g
LIB         := `pkg-config --cflags --libs gtk+-3.0 gtkmm-3.0 glibmm-2.4`
INC         := -I$(INCDIR) -I/usr/local/include
INCDEP      := -I$(INCDIR)


#---------------------------------------------------------------------------------
#DO NOT EDIT BELOW THIS LINE
#---------------------------------------------------------------------------------
SOURCES     := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS     := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.$(OBJEXT)))

#Defauilt Make
all: resources $(TARGET)

#Remake
remake: cleaner all

#Copy Resources from Resources Directory to Target Directory
resources: directories
	if [ -d $(RESDIR) ]; then cp $(RESDIR)/* $(TARGETDIR)/; fi;

#Make the Directories
directories:
	@mkdir -p $(TARGETDIR)
	@mkdir -p $(BUILDDIR)

#Clean only Objecst
clean:
	@$(RM) -rf $(BUILDDIR)

#Full Clean, Objects and Binaries
cleaner: clean
	@$(RM) -rf $(TARGETDIR)

#Pull in dependency info for *existing* .o files
-include $(OBJECTS:.$(OBJEXT)=.$(DEPEXT))

#Link
$(TARGET): $(OBJECTS)
	$(CC) -o $(TARGETDIR)/$(TARGET) $^ $(LIB)

#Compile
$(BUILDDIR)/%.$(OBJEXT): $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INC) -c -o $@ $<
	@$(CC) $(CFLAGS) $(INCDEP) -MM $(SRCDIR)/$*.$(SRCEXT) > $(BUILDDIR)/$*.$(DEPEXT)
	@cp -f $(BUILDDIR)/$*.$(DEPEXT) $(BUILDDIR)/$*.$(DEPEXT).tmp
	@sed -e 's|.*:|$(BUILDDIR)/$*.$(OBJEXT):|' < $(BUILDDIR)/$*.$(DEPEXT).tmp > $(BUILDDIR)/$*.$(DEPEXT)
	@sed -e 's/.*://' -e 's/\\$$//' < $(BUILDDIR)/$*.$(DEPEXT).tmp | fmt -1 | sed -e 's/^ *//' -e 's/$$/:/' >> $(BUILDDIR)/$*.$(DEPEXT)
	@rm -f $(BUILDDIR)/$*.$(DEPEXT).tmp



# PROGRAM_NAME = main

# CXX = g++
# LIBARIES = `pkg-config --cflags --libs gtk+-3.0 gtkmm-3.0 glibmm-2.4`

# SOURCE_DIRECTORY = src
# OBJECT_DIRECTORY = .build


# # What this does is that first it get a list of cpp files from $(SRC_DIR), removes the path, then
# # replaces all cpp suffixes for o and lastly add `../$(BIN_DIR)/' in front of each item.
# SOURCE_FILES := $(wildcard $(SOURCE_DIRECTORY)/*.cpp)
# OBJECT_FILES := $(patsubst $(SOURCE_DIRECTORY)/%,%,$(SOURCE_FILES))
# OBJECT_FILES := $(patsubst %.cpp,%.o,$(OBJECT_FILES))
# OBJECT_FILES := $(addprefix $(OBJECT_DIRECTORY)/,$(OBJECT_FILES))


# all: $(OBJECT_FILES)
# 	@echo "Building target: $@"
# 	@echo "Invoking: Cross G++ Linker"
# 	$(CXX) -o $(PROGRAM_NAME) $(OBJECT_FILES) $(LIBARIES)


# $(OBJECT_FILES): $(SOURCE_FILES)
# 	@echo "Building target: $@"
# 	@echo "Invoking: Cross G++ on Object Files"
# 	$(CXX) -c -I$(SOURCE_DIRECTORY) $< -o $@ $(LIBARIES)


# $(OBJECT_DIRECTORY)/%.o: $(SOURCE_DIRECTORY)/%.c
# 	@echo "Building target: $@"
# 	@echo "Invoking: Cross G++ on Object Files"
# 	$(CXX) -c -o $@ $<


