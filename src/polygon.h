#ifndef GTKMM_APP_WIREFRAME
#define GTKMM_APP_WIREFRAME

#include "drawableobject.h"
#include "coordinate.h"

class Polygon : public DrawableObject
{
public:
  Polygon(std::string name, std::list<Coordinate*> coordinates, std::list<Coordinate*> coordinates_in_window);
  ~Polygon();
};
#endif // GTKMM_APP_WIREFRAME
