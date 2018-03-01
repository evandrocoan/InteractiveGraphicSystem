#ifndef GTKMM_APP_VIEWPORT
#define GTKMM_APP_VIEWPORT

#include <gtkmm/drawingarea.h>
#include <gdkmm/rgba.h>
#include <cairomm/context.h>
#include <list>
#include "viewwindow.h"
#include "coordinate.h"

class Viewport : public Gtk::DrawingArea
{

public:
  Viewport();
  virtual ~Viewport();
  ViewWindow * getViewWindow();

protected:
  bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;
  Gtk::Allocation* allocation;
  ViewWindow* viewWindow = NULL;
  int Xvpmin, Yvpmin, Xvpmax, Yvpmax;
  void updateViewport(Gtk::Allocation allocation);
  Coordinate convertCoordinateFromWindow(Coordinate cord);

};

#endif
