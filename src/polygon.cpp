#include "polygon.h"

Polygon::Polygon(string name, list<Coordinate*> Coordinates)
    : DrawableObject(name, Coordinates)
{
  // nothing for while
}

Polygon::~Polygon()
{
}

list<Coordinate*> Polygon::getCoordinates()
{
  return Coordinates;
}
