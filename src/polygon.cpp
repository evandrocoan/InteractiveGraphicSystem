#include "polygon.h"

Polygon::Polygon(string name, list<Coordinate*> coordinates)
    : DrawableObject(name, coordinates)
{
  // nothing for while
}

Polygon::~Polygon()
{
}
