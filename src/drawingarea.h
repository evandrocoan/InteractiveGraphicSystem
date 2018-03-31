#ifndef GTKMM_APP_DRAWINGAREA
#define GTKMM_APP_DRAWINGAREA

#include <list>
#include <string>
#include <iomanip>

#include <gtkmm/drawingarea.h>
#include <gdkmm/rgba.h>
#include <cairomm/context.h>

#include "traits.h"
#include "debugger.h"
#include "subject_controller.h"

#include "point.h"
#include "line.h"
#include "polygon.h"
#include "coordinate.h"

#include "viewport.h"
#include "viewwindow.h"
#include "displayfile.h"

/**
 * The Drawing Area Widget
 * https://developer.gnome.org/gtkmm-tutorial/stable/chapter-drawingarea.html.en
 *
 * Drawing Straight Lines
 * https://developer.gnome.org/gtkmm-tutorial/stable/sec-cairo-drawing-lines.html.en#cairo-example-lines
 */
class DrawingArea : public Gtk::DrawingArea
{
public:
  DrawingArea();
  virtual ~DrawingArea();

  void addPoint(std::string name, int, int);
  void addLine(std::string name, int, int, int, int);
  void addPolygon(std::string name, std::vector<big_double>);
  void removeObject(std::string name);

  void zoom(big_double step = 50);
  void move(big_double horizontal_step = 50, big_double vertical_step = 50);
  void rotate(Coordinate coordinate);

  void updateObjectCoordinates();
  void apply(std::string object_name, Transformation&);
  Coordinate convertCoordinateToViewPort(Coordinate&);

  std::list<std::string> getNamesList();
  std::list<DrawableObject*> getObjectsList();

  void disconnectObserver();
  Signal<>::Connection addObserver(const Signal<>::Callback&);
  friend std::ostream& operator<<(std::ostream &output, const DrawingArea &object);

protected:
  ViewPort   viewPort;
  ViewWindow viewWindow;

  bool isCentered;
  DisplayFile displayFile;

  Signal<> callObservers;
  Signal<>::Connection _connection;

  bool on_draw(const Cairo::RefPtr<Cairo::Context>&) override;
  void on_my_size_allocate(Gtk::Allocation&);

  void addObject(DrawableObject*);
  void updateViewPortSize (big_double width, big_double height);

private:
  void draw_xy_axes();
};

#endif // GTKMM_APP_DRAWINGAREA
