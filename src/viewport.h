#ifndef GTKMM_APP_VIEWPORT
#define GTKMM_APP_VIEWPORT

#include <list>
#include <string>

#include <gtkmm/drawingarea.h>
#include <gdkmm/rgba.h>
#include <cairomm/context.h>

#include "viewwindow.h"
#include "displayfile.h"
#include "viewportobserver.h"
#include "coordinate.h"
#include "debugger.h"

/**
 * The Drawing Area Widget
 * https://developer.gnome.org/gtkmm-tutorial/stable/chapter-drawingarea.html.en
 *
 * Drawing Straight Lines
 * https://developer.gnome.org/gtkmm-tutorial/stable/sec-cairo-drawing-lines.html.en#cairo-example-lines
 */
class ViewPort : public Gtk::DrawingArea
{
public:
  ViewPort();
  virtual ~ViewPort();

  void addObject(DrawableObject*);
  void removeObject(std::string name);

  void addObserver(ViewPortObserver*);
  std::list<std::string> getNamesList();

  void zoom_in (float scale = 1.5);
  void zoom_out(float scale = 1.5);

  void move_up   (int length = 10);
  void move_down (int length = 10);
  void move_left (int length = 10);
  void move_right(int length = 10);

protected:
  ViewWindow        viewWindow;
  DisplayFile       displayFile;
  ViewPortObservers viewPortObservers;

  int xVpmin;
  int yVpmin;
  int xVpmax;
  int yVpmax;

  bool       on_draw(const Cairo::RefPtr<Cairo::Context>&) override;
  void       updateViewport(Gtk::Allocation);
  Coordinate convertCoordinateFromWindow(Coordinate);
};

#endif
