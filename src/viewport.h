#ifndef GTKMM_APP_VIEWPORT
#define GTKMM_APP_VIEWPORT

#include <gtkmm/drawingarea.h>
#include <gdkmm/rgba.h>
#include <cairomm/context.h>
#include <list>
#include "viewwindow.h"
#include "displayfile.h"
#include "coordinate.h"

class Viewport : public Gtk::DrawingArea
{

public:
  Viewport();
  virtual ~Viewport();

  ViewWindow * getViewWindow();
  DisplayFile* getDisplayFile();

protected:
  ViewWindow* viewWindow = NULL;

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
