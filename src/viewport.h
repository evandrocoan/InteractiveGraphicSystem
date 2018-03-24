#ifndef GTKMM_APP_VIEWPORT
#define GTKMM_APP_VIEWPORT

#include <iostream>
#include <iomanip>

#include "polygon.h"

/**
 * The Drawing Area Widget
 * https://developer.gnome.org/gtkmm-tutorial/stable/chapter-drawingarea.html.en
 *
 * Drawing Straight Lines
 * https://developer.gnome.org/gtkmm-tutorial/stable/sec-cairo-drawing-lines.html.en#cairo-example-lines
 */
class ViewPort
{
public:
  ViewPort();
  virtual ~ViewPort();

  std::list<Coordinate*> getCoordinates();
  void updateClippingWindowSize(int width, int height);
  friend std::ostream& operator<<(std::ostream &output, const ViewPort &object);

  int xMin;
  int yMin;
  int xMax;
  int yMax;

protected:
  Polygon clippingWindow;

  int _drawing_area_width;
  int _drawing_area_height;
};

#endif
