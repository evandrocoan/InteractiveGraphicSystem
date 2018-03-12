#ifndef GTKMM_APP_VIEWPORT
#define GTKMM_APP_VIEWPORT

#include <list>

#include <gtkmm/drawingarea.h>
#include <gdkmm/rgba.h>
#include <cairomm/context.h>

#include "viewwindow.h"
#include "displayfile.h"
#include "viewportobserver.h"
#include "coordinate.h"
#include "debugger.h"

class Viewport : public Gtk::DrawingArea
{
public:
  Viewport();
  virtual ~Viewport();

  Viewwindow*  getViewwindow();
  DisplayFile* getDisplayFile();
  void         addObject(DrawableObject*);
  void         removeObject(std::string name);
  void         addObserver(ViewportObserver*);

protected:
  Viewwindow        viewwindow;
  DisplayFile       displayFile;
  ViewportObservers viewportObservers;

  int Xvpmin;
  int Yvpmin;
  int Xvpmax;
  int Yvpmax;

  bool       on_draw(const Cairo::RefPtr<Cairo::Context>&) override;
  void       updateViewport(Gtk::Allocation);
  Coordinate convertCoordinateFromWindow(Coordinate);
};

#endif
