#include "polygon.h"

Polygon::Polygon(std::string name, std::list<Coordinate*> coordinates)
    : DrawableObject(name, coordinates)
{
  // nothing for while
}

Polygon::~Polygon()
{
}

void Polygon::addCoordinate(Coordinate* coordinate)
{
  this->coordinates.push_back(coordinate);
}

void Polygon::clearCoordinates()
{
  this->coordinates.clear();
}
