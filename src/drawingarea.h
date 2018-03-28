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

  Signal<>::Connection addObserver(const Signal<>::Callback&);
  void disconnectObserver();

  void addPoint(std::string name, int, int);
  void addLine(std::string name, int, int, int, int);
  void addPolygon(std::string name, std::vector<GTKMM_APP_MATRICES_DATATYPE>);

  void removeObject(std::string name);
  std::list<std::string> getNamesList();

  void zoom_in (float scale = 1.5);
  void zoom_out(float scale = 1.5);

  void move_up   (int length = 10);
  void move_down (int length = 10);
  void move_left (int length = 10);
  void move_right(int length = 10);

  void rotate_left (GTKMM_APP_MATRICES_DATATYPE angle = 10);
  void rotate_right(GTKMM_APP_MATRICES_DATATYPE angle = 10);

  void apply(std::string object_name, Transformation&);
  void updateClipping();

  Coordinate convertCoordinateFromWindow(Coordinate&);

  friend std::ostream& operator<<(std::ostream &output, const DrawingArea &object);

  Coordinate coordinateWindowToViewPort(Coordinate&);
  Coordinate coordinateWorldToWindow(Coordinate&);

  std::list<Coordinate*> listCoordinateWorldToWindow(std::list<Coordinate*> coordinates);
  std::list<DrawableObject*> getObjectsList();

protected:
  ViewPort   viewPort;
  ViewWindow viewWindow;

  bool isCentered;
  DisplayFile displayFile;

  Signal<> callObservers;
  Signal<>::Connection _connection;

  void addObject(DrawableObject*);
  bool on_draw(const Cairo::RefPtr<Cairo::Context>&) override;

  void updateViewPort     (Gtk::Allocation&);
  void on_my_size_allocate(Gtk::Allocation&);

  void computeWindowCoordinate(Transformation transformation);

private:

  void on_init();
  bool onInit = true;
};

#endif // GTKMM_APP_DRAWINGAREA
