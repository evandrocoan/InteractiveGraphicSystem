#include "polygon.h"

Polygon::Polygon(std::string name, std::list<Coordinate*> coordinates)
    : DrawableObject(name, coordinates)
{
  // nothing for while
}

Polygon::~Polygon()
{
}
