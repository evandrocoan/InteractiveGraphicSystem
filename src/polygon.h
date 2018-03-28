#ifndef GTKMM_APP_POLYGON
#define GTKMM_APP_POLYGON

#include "drawableobject.h"
#include "coordinate.h"

class Polygon : public DrawableObject
{
public:
  Polygon(std::string name, std::list<Coordinate*> coordinates);
  ~Polygon();
};

#endif // GTKMM_APP_POLYGON
