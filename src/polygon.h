#ifndef GTKMM_APP_POLYGON
#define GTKMM_APP_POLYGON

#include "drawableobject.h"
#include "coordinate.h"

class Polygon : public DrawableObject
{
public:
  Polygon(std::string name, std::list<Coordinate*> coordinates);
  ~Polygon();

  void addCoordinate(Coordinate* coordinate);
  void clearCoordinates();
};

#endif // GTKMM_APP_POLYGON
