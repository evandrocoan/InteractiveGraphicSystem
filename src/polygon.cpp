#include "polygon.h"

Polygon::Polygon(std::string name, std::list<Coordinate*> worldCoordinates) :
      DrawableObject(name, worldCoordinates)
{
}

Polygon::~Polygon()
{
}
