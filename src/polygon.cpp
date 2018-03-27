#include "polygon.h"

Polygon::Polygon(std::string name, std::list<Coordinate*> coordinates, std::list<Coordinate*> coordinates_in_window )
    : DrawableObject(name, coordinates, coordinates_in_window)
{
  // nothing for while
}

Polygon::~Polygon()
{
}
