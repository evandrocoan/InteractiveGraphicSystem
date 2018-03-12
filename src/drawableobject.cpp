#include "drawableobject.h"

DrawableObject::DrawableObject(std::string name) :
      name(name)
{
}

DrawableObject::DrawableObject(std::string name, std::list<Coordinate*> coordinates) :
      name(name),
      coordinates(coordinates)
{
}

DrawableObject::~DrawableObject()
{
}

std::string DrawableObject::getName()
{
  return this->name;
}

std::list<Coordinate*> DrawableObject::getCoordinates()
{
  return this->coordinates;
}
