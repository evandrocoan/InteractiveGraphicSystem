#ifndef GTKMM_APP_RECT
#define GTKMM_APP_RECT

#include "drawableobject.h"
#include "coordinate.h"

class Line : public DrawableObject
{
public:
  Line(std::string name, Coordinate* line_cord1, Coordinate* line_cord2);
  ~Line();
};
#endif // GTKMM_APP_RECT
