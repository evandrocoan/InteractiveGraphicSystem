#ifndef GTKMM_APP_LINE
#define GTKMM_APP_LINE

#include "coordinate.h"
#include "drawableobject.h"

class Line : public DrawableObject
{
public:
  Line(std::string name, Coordinate* line_cord1, Coordinate* line_cord2);
  ~Line();

  virtual void updateClippingCoordinates(ViewPort& axes);
};

#endif // GTKMM_APP_LINE
