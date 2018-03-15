#include "coordinate.h"

Coordinate::Coordinate(long int x, long int y, long int z = 1) :
      coordinates(x, y, z)
{
}

Coordinate::~Coordinate()
{
}

void Coordinate::apply(Transformation transformation)
{
  this->coordinates = transformation.apply(coordinates);
}

long int Coordinate::getx()
{
  return this->coordinates[0];
}

long int Coordinate::gety()
{
  return this->coordinates[1];
}

long int Coordinate::getz()
{
  return this->coordinates[2];
}

