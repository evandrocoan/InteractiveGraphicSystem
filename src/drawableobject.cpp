#include "drawableobject.h"

DrawableObject::DrawableObject(std::string name) :
      name(name)
{
}

DrawableObject::DrawableObject(std::string name, std::list<Coordinate*> worldCoordinates) :
      name(name),
      worldCoordinates(worldCoordinates)
{
  LOG(4, "Deep coping initializing the other attributes");

  for( auto coordinate : worldCoordinates )
  {
    this->viewWindowCoordinates.push_back(new Coordinate(*coordinate));
    this->clippingCoordinates.push_back(new Coordinate(*coordinate));
  }
}

DrawableObject::~DrawableObject()
{
	this->destroyList(worldCoordinates);
}

/**
 * https://stackoverflow.com/questions/307082/cleaning-up-an-stl-list-vector-of-pointers
 * Cleaning up an STL list/vector of pointers
 */
void DrawableObject::destroyList(std::list<Coordinate*>& worldCoordinates)
{
  while(!worldCoordinates.empty())
  {
    // delete worldCoordinates.front();
    worldCoordinates.pop_front();
  }
}

std::string DrawableObject::getName()
{
  return this->name;
}

std::list<Coordinate*>& DrawableObject::getWorldCoordinates()
{
  return this->worldCoordinates;
}

std::list<Coordinate*>& DrawableObject::getClippingCoordinates()
{
  return this->clippingCoordinates;
}

std::list<Coordinate*>& DrawableObject::getViewWindowCoordinates()
{
  return this->viewWindowCoordinates;
}

void DrawableObject::addWorldCoordinate(Coordinate* worldCoordinates)
{
  this->worldCoordinates.push_back(worldCoordinates);
  this->viewWindowCoordinates.push_back(worldCoordinates);
  this->clippingCoordinates.push_back(worldCoordinates);
}

void DrawableObject::clearWorldCoordinates()
{
  this->worldCoordinates.clear();
  this->viewWindowCoordinates.clear();
  this->clippingCoordinates.clear();
}

Coordinate DrawableObject::getGeometricCenter(const std::list<Coordinate*>& coordinates)
{
  int coordinatesCount = coordinates.size();

  big_double x_axis = 0;
  big_double y_axis = 0;
  big_double z_axis = 0;

  for(auto coordinate : coordinates)
  {
    x_axis += coordinate->x;
    y_axis += coordinate->y;
    z_axis += coordinate->z;
  }

  return Coordinate(x_axis/coordinatesCount, y_axis/coordinatesCount, z_axis/coordinatesCount);
}

/**
 * Prints a more beauty version of the object when called on `std::cout<< object << std::end;`
 */
std::ostream& operator<<( std::ostream &output, const DrawableObject &object )
{
  object.printMyself(output);
  return output;
}

void DrawableObject::printMyself(std::ostream& output) const
{
  unsigned int size;
  unsigned int index;
  unsigned int index_external = 0;
  std::list< std::list<Coordinate*> > coordinates_lists;

  coordinates_lists.push_back(this->worldCoordinates);
  coordinates_lists.push_back(this->viewWindowCoordinates);
  coordinates_lists.push_back(this->clippingCoordinates);

  output << this->name;

  for( auto coordinates_list : coordinates_lists )
  {
    index = 0;
    size = coordinates_list.size() - 1;

    output << "_" << index_external << "(";

    for( auto coordinate : coordinates_list )
    {
      output << *coordinate;

      if( index != size )
      {
        output << ", ";
      }

      index++;
    }

    index_external++;
    output << ")";
  }
}

void DrawableObject::apply(Transformation &transformation)
{
  LOG(8, "Entering...");
  auto coordinates = this->getWorldCoordinates();
  auto geometricCenter = DrawableObject::getGeometricCenter(coordinates);
  transformation.set_geometric_center(geometricCenter);

  for(auto coordinate : coordinates)
  {
    transformation.apply(*coordinate);
  }
}

void DrawableObject::updateWindowCoordinates(ViewWindow &viewwindow)
{
  LOG(8, "Entering... %s", viewwindow);
  Coordinate* new_coordinate;
  auto coordinates = this->getWorldCoordinates();
  DrawableObject::destroyList(this->viewWindowCoordinates);

  for(auto coordinate : coordinates)
  {
    new_coordinate = new Coordinate(*coordinate);
    viewwindow.apply(*new_coordinate);
    this->viewWindowCoordinates.push_back(new_coordinate);
  }
}

void DrawableObject::updateClippingCoordinates(ViewPort& viewport)
{
  LOG(4, "Generic clipping update... %s", viewport);
  this->clippingCoordinates = this->worldCoordinates;
}
