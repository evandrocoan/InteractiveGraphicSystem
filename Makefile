


PROGRAM_NAME = main

CXX = g++
LIBARIES = `pkg-config --cflags --libs glibmm-2.4 gtk+-3.0 gtkmm-3.0`
BINARY_FILES_FOLDER = .build


CXX_FILES = src/displayfile.cpp src/drawoptionsbox.cpp src/line.cpp src/main.cpp src/mainwindow.cpp src/point.cpp src/polygon.cpp src/viewport.cpp src/viewwindow.cpp src/addobjectwindow.cpp

HXX_FILES = src/addobjectwindow.h src/coordinate.h src/displayfile.h src/drawableobject.h src/drawoptionsbox.h src/line.h src/mainwindow.h src/point.h src/polygon.h src/viewport.h src/viewwindow.h


OBJECT_FILES = $(CXX_FILES:.cpp=.o)


all: $(CXX_FILES) $(HXX_FILES)
	$(CXX) -o $(PROGRAM_NAME) $(HXX_FILES) $(CXX_FILES) $(LIBARIES)




