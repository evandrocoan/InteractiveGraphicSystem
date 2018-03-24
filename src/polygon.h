#ifndef GTKMM_APP_WIREFRAME
#define GTKMM_APP_WIREFRAME

#include "drawableobject.h"
#include "coordinate.h"

class Polygon : public DrawableObject
{
public:
  Polygon(std::string name, std::list<Coordinate*> coordinates);
  ~Polygon();

  void addCoordinate(Coordinate*);
  void clearCoordinates();
};
#endif // GTKMM_APP_WIREFRAME
