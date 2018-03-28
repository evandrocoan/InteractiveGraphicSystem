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
    this->clippedCoordinates.push_back(new Coordinate(*coordinate));
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
void DrawableObject::destroyList(std::list<Coordinate*> worldCoordinates)
{
  while(!worldCoordinates.empty())
  {
    // delete worldCoordinates.front();
    worldCoordinates.pop_front();
  }
}

void DrawableObject::updateClipping(ViewPort& axes)
{
  LOG(4, "Generic clipping update... %s", axes);
  this->clippedCoordinates = this->worldCoordinates;
}

std::string DrawableObject::getName()
{
  return this->name;
}

std::list<Coordinate*>& DrawableObject::getWorldCoordinates()
{
  return this->worldCoordinates;
}

std::list<Coordinate*>& DrawableObject::getClippedCoordinates()
{
  return this->clippedCoordinates;
}

std::list<Coordinate*>& DrawableObject::getViewWindowCoordinates()
{
  return this->viewWindowCoordinates;
}

Coordinate* DrawableObject::getGeometricCenter()
{
  auto worldCoordinates = this->getWorldCoordinates();
  int coordinatesCount = worldCoordinates.size();

  long int x_axis = 0;
  long int y_axis = 0;
  long int z_axis = 0;

  for(auto coordinate : worldCoordinates)
  {
    x_axis += coordinate->getx();
    y_axis += coordinate->gety();
    z_axis += coordinate->getz();
  }

  return new Coordinate(x_axis/coordinatesCount, y_axis/coordinatesCount, z_axis/coordinatesCount);
}

void DrawableObject::setViewWindowCoordinates(std::list<Coordinate*> viewWindowCoordinates)
{
  this->destroyList(this->viewWindowCoordinates);
  this->viewWindowCoordinates= viewWindowCoordinates;
}

void DrawableObject::addWorldCoordinate(Coordinate* worldCoordinates)
{
  this->worldCoordinates.push_back(worldCoordinates);
  this->viewWindowCoordinates.push_back(worldCoordinates);
  this->clippedCoordinates.push_back(worldCoordinates);
}

void DrawableObject::clearWorldCoordinates()
{
  this->worldCoordinates.clear();
  this->viewWindowCoordinates.clear();
  this->clippedCoordinates.clear();
}

Coordinate* DrawableObject::getWindowGeometricCenter()
{
  auto worldCoordinates = this->getViewWindowCoordinates();
  int coordinatesCount = worldCoordinates.size();

  long int x_axis = 0;
  long int y_axis = 0;
  long int z_axis = 0;

  for(auto coordinate : worldCoordinates)
  {
    x_axis += coordinate->getx();
    y_axis += coordinate->gety();
    z_axis += coordinate->getz();
  }

  return new Coordinate(x_axis/coordinatesCount, y_axis/coordinatesCount, z_axis/coordinatesCount);
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
  std::list< std::list<Coordinate*> > coordinates_lists;

  coordinates_lists.push_back(this->worldCoordinates);
  coordinates_lists.push_back(this->clippedCoordinates);
  coordinates_lists.push_back(this->viewWindowCoordinates);

  output << this->name;

  for( auto coordinates_list : coordinates_lists )
  {
    index = 0;
    size = coordinates_list.size() - 1;

    output << "-" << index << "(";

    for( auto coordinate : coordinates_list )
    {
      output << *coordinate;

      if( index != size )
      {
        output << ", ";
      }

      index++;
    }

    output << ")";
  }
}

void DrawableObject::apply(Transformation &transformation)
{
  LOG(8, "Entering apply");
  auto worldCoordinates = this->getWorldCoordinates();
  auto geometricCenter = this->getGeometricCenter();

  transformation.set_geometric_center(*geometricCenter);
  delete geometricCenter;

  for(auto coordinate : worldCoordinates)
  {
    transformation.apply(*coordinate);
  }
}

void DrawableObject::applyInWindow(Transformation &transformation)
{

  LOG(8, "Entering applyInWindow");
  auto worldCoordinates = this->getViewWindowCoordinates();
  auto geometricCenter = this->getWindowGeometricCenter();

  transformation.set_geometric_center(*geometricCenter);
  delete geometricCenter;

  for(auto coordinate : worldCoordinates)
  {
    transformation.apply(*coordinate);
  }

}
