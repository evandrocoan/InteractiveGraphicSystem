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
    x_axis += coordinate->getx();
    y_axis += coordinate->gety();
    z_axis += coordinate->getz();
  }

  return Coordinate(x_axis/coordinates_count, y_axis/coordinates_count, z_axis/coordinates_count);
}

std::list<Coordinate*> DrawableObject::getCoordinates()
{
  return this->coordinates;
}

/**
 * Prints a more beauty version of the matrix when called on `std::cout<< matrix << std::end;`
 */
std::ostream& operator<<( std::ostream &output, const DrawableObject &object )
{
  for( auto coordinate : object.coordinates )
  {
    output << *coordinate << " ";
  }

  return output;
}

void DrawableObject::apply(Transformation* transformation)
{
  auto coordinates = this->getCoordinates();
  transformation->set_geometric_center(this->get_geometric_center());

  for(auto coordinate : coordinates)
  {
    transformation->apply(coordinate);
  }
}
