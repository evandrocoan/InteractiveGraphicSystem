#ifndef GTKMM_APP_VIEWPORT
#define GTKMM_APP_VIEWPORT

#include <list>
#include <string>

#include <gtkmm/drawingarea.h>
#include <gdkmm/rgba.h>
#include <cairomm/context.h>

#include "viewwindow.h"
#include "displayfile.h"
#include "subject_controller.h"
#include "coordinate.h"
#include "debugger.h"
#include "line.h"

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

  std::list<std::string> getNamesList();
  void apply(std::string object_name, Transformation&);

  void zoom_in (float scale = 1.5);
  void zoom_out(float scale = 1.5);

  void move_up   (int length = 10);
  void move_down (int length = 10);
  void move_left (int length = 10);
  void move_right(int length = 10);

  void move_center();

  void rotate_left (long double angle = 10);
  void rotate_right(long double angle = 10);

  Signal<>::Connection addObserver(const Signal<>::Callback&);
  Coordinate convertCoordinateFromWindow(Coordinate&);
  Coordinate coordinateWindowToViewPort(Coordinate&);
  Coordinate coordinateWorldToWindow(Coordinate&);

  std::list<DrawableObject*> getObjectsList();

protected:
  ViewWindow  viewWindow;
  DisplayFile displayFile;
  Signal<>    observerController;

  int xVpmin;
  int yVpmin;
  int xVpmax;
  int yVpmax;

  bool on_draw(const Cairo::RefPtr<Cairo::Context>&) override;
  void updateViewport(Gtk::Allocation&);

  void computeWindowCoordinate(Transformation transformation);

private:

  void on_init();
  bool onInit = true;
};

#endif
