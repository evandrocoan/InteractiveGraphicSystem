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

Coordinate* DrawableObject::get_geometric_center()
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

  return new Coordinate(x_axis/coordinates_count, y_axis/coordinates_count, z_axis/coordinates_count);
}

std::list<Coordinate*>& DrawableObject::getCoordinates()
{
  return this->coordinates;
}

/**
 * Prints a more beauty version of the matrix when called on `std::cout<< matrix << std::end;`
 */
std::ostream& operator<<( std::ostream &output, const DrawableObject &object )
{
  output << object.name << "(";

  unsigned int index = 0;
  unsigned int size = object.coordinates.size() - 1;

  for( auto coordinate : object.coordinates )
  {
    output << *coordinate;

    if( index != size )
    {
      output << ", ";
    }

    index++;
  }

  output << ")";
  return output;
}

void DrawableObject::apply(Transformation &transformation)
{
  LOG(2, "Leaving...");
  auto coordinates = this->getCoordinates();
  LOG(2, "Leaving...");
  auto geometric_center = this->get_geometric_center();

  LOG(2, "Leaving...");
  transformation.set_geometric_center(*geometric_center);
  LOG(2, "Leaving...");
  delete geometric_center;
  LOG(2, "Leaving...");

  for(auto coordinate : coordinates)
  {
  LOG(2, "Leaving...");
    transformation.apply(*coordinate);
  LOG(2, "Leaving...");
  }

  LOG(2, "Leaving...");
}
