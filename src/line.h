#ifndef GTKMM_APP_LINE
#define GTKMM_APP_LINE

#include "coordinate.h"
#include "drawableobject.h"

class Line : public DrawableObject
{
public:
  Line(std::string name, Coordinate* line_cord1, Coordinate* line_cord2) :
      DrawableObject(name)
  {
    coordinates.push_back(line_cord1);
    coordinates.push_back(line_cord2);
  }

  ~Line()
  {
  }

  virtual void updateClipping(ClippingWindow& axes)
  {
    LOG(4, "Line clipping update... %s", axes);
    this->clipped_coordinates = this->coordinates;
  }
};

#endif // GTKMM_APP_LINE
