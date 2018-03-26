#ifndef GTKMM_APP_POLYGON
#define GTKMM_APP_POLYGON

#include "drawableobject.h"
#include "coordinate.h"

class Polygon : public DrawableObject
{
public:
  Polygon(std::string name, std::list<Coordinate*> coordinates) :
      DrawableObject(name, coordinates)
  {
  }

  ~Polygon()
  {
  }

  void addCoordinate(Coordinate* coordinate)
  {
    this->coordinates.push_back(coordinate);
  }

  void clearCoordinates()
  {
    this->coordinates.clear();
  }
};
#endif // GTKMM_APP_POLYGON
