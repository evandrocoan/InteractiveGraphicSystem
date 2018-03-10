#ifndef GTKMM_APP_VIEWPORT
#define GTKMM_APP_VIEWPORT

#include <gtkmm/drawingarea.h>
#include <gdkmm/rgba.h>
#include <cairomm/context.h>
#include <list>
#include "viewwindow.h"
#include "displayfile.h"
#include "coordinate.h"
#include "debugger.h"

class Viewport : public Gtk::DrawingArea
{
public:
  Viewport();
  virtual ~Viewport();

  Viewwindow * getViewwindow();
  DisplayFile* getDisplayFile();

protected:
  Viewwindow* viewwindow = NULL;

  DisplayFile displayFile;
  Gtk::Allocation* allocation;

  int Xvpmin;
  int Yvpmin;

  int Xvpmax;
  int Yvpmax;

  bool       on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;
  void       updateViewport(Gtk::Allocation allocation);
  Coordinate convertCoordinateFromWindow(Coordinate cord);
};

#endif
