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

Coordinate DrawableObject::get_geometric_center()
{
  auto coordinates      = this->getCoordinates();
  int coordinates_count = coordinates.size();

  long int x_axis = 0;
  long int y_axis = 0;
  long int z_axis = 0;

  for(auto coordinate : coordinates)
  {
    x_axis += coordinate.getx();
    y_axis += coordinate.gety();
    z_axis += coordinate.getz();
  }

  return Coordinate(x_axis/coordinates_count, y_axis/coordinates_count, z_axis/coordinates_count)
}

std::list<Coordinate*> DrawableObject::getCoordinates()
{
  return this->coordinates;
}

void DrawableObject::apply(Transformation transformation)
{
  transformation.set_geometric_center(this->get_geometric_center())
  auto coordinates = this->getCoordinates();

  for(auto coordinate : coordinates)
  {
    coordinate.apply(transformation);
  }
}
