#include "drawableobject.h"

DrawableObject::DrawableObject(std::string name, std::vector<Coordinate*> _worldCoordinates) :
      DrawableObject(name, _worldCoordinates, Coordinate(0, 0, 0))
{
}

DrawableObject::DrawableObject(
          std::string name,
          std::vector<Coordinate*> _worldCoordinates,
          Coordinate _borderColor,
          Coordinate _fillingColor,
          bool _isVisibleOnGui ) :
      name(name),
      _isDrawable(false),
      _isVisibleOnGui(_isVisibleOnGui),
      _borderColor(_borderColor),
      _fillingColor(_fillingColor),
      _worldCoordinates(_worldCoordinates)
{
  // LOG(4, "Deep coping initializing the coordinate attributes");

  if( _worldCoordinates.size() < 1 )
  {
    std::string error = tfm::format( "ERROR! An object must have at least 1 coordinate: %s", *this );
    LOG( 1, "%s", error );
    throw std::runtime_error( error );
  }

  if( name.empty() )
  {
    std::ostringstream contents;
    for( auto value : _worldCoordinates ) contents << *value << ", ";

    std::string error = tfm::format( "The loaded object has no name! \n%s", contents.str() );

    LOG( 1, "%s", error );
    throw std::runtime_error( error );
  }

  for( auto coordinate : _worldCoordinates )
  {
    this->_windowCoordinates.push_back(new Coordinate(*coordinate));
    this->_clippingCoordinates.push_back(new Coordinate(*coordinate));
  }
}

DrawableObject::~DrawableObject()
{
  this->destroyList(this->_worldCoordinates);
  this->destroyList(this->_windowCoordinates);
	this->destroyList(this->_clippingCoordinates);
}

std::string DrawableObject::getName() const
{
  return this->name;
}

const std::vector<Coordinate*>& DrawableObject::worldCoordinates() const
{
  return this->_worldCoordinates;
}

const std::vector<Coordinate*>& DrawableObject::windowCoordinates() const
{
  return this->_windowCoordinates;
}

const std::vector<Coordinate*>& DrawableObject::clippingCoordinates() const
{
  return this->_clippingCoordinates;
}

const Coordinate DrawableObject::getGeometricCenter()
{
  int coordinatesCount = _worldCoordinates.size();

  big_double x_axes = 0;
  big_double y_axes = 0;
  big_double z_axes = 0;

  for(auto coordinate : _worldCoordinates)
  {
    x_axes += coordinate->x;
    y_axes += coordinate->y;
    z_axes += coordinate->z;
  }

  return Coordinate(x_axes/coordinatesCount, y_axes/coordinatesCount, z_axes/coordinatesCount);
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
  std::vector< std::vector<Coordinate*> > coordinates_lists;

  coordinates_lists.push_back(this->_worldCoordinates);
  // coordinates_lists.push_back(this->_windowCoordinates);
  // coordinates_lists.push_back(this->_clippingCoordinates);

  output << this->name;

  for( auto coordinates_list : coordinates_lists )
  {
    index = 0;
    size = coordinates_list.size() - 1;

    output << ", " << index_external << "[";

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
    output << "]";
  }
}

void DrawableObject::apply(Transformation &transformation)
{
  LOG(8, "...");
  auto coordinates = this->worldCoordinates();
  std::map<Coordinate*, Coordinate*> coordinatesMap;

  auto geometricCenter = getGeometricCenter();
  transformation.set_geometric_center(geometricCenter);

  for(auto coordinate : coordinates) {
    auto iterator = coordinatesMap.find( coordinate );

    if( iterator == coordinatesMap.end() )
    {
      coordinatesMap[coordinate] = coordinate;
      transformation.apply(*coordinate);
    }
  }
}

void DrawableObject::updateWindowCoordinates(const Transformation& transformation)
{
  LOG(8, "...");
  Coordinate* new_coordinate;
  DrawableObject::destroyList(this->_windowCoordinates);

  if( transformation.projectionDistance )
  {
    for(auto coordinate : this->_worldCoordinates) {
      new_coordinate = new Coordinate(*coordinate);
      transformation.apply(*new_coordinate);

      if( transformation.projectionDistance - 0.001 < 0 && transformation.projectionDistance + 0.001 > 0 ) {
        LOG( 8, "%s transformation.projectionDistance is %s", getName(), transformation.projectionDistance );
      }
      else {
        if( new_coordinate->z - 0.001 < 0 && new_coordinate->z + 0.001 > 0 ) {
          LOG( 8, "%s new_coordinate->z is %s", getName(), new_coordinate->z );
        }
        else {
          new_coordinate->x = new_coordinate->x / ( new_coordinate->z / transformation.projectionDistance );
          new_coordinate->y = new_coordinate->y / ( new_coordinate->z / transformation.projectionDistance );
          new_coordinate->z = transformation.projectionDistance;
        }
      }

      transformation.posTransformation->apply(*new_coordinate);
      this->_windowCoordinates.push_back(new_coordinate);
    }
  }
  else {
    for(auto coordinate : this->_worldCoordinates)
    {
      new_coordinate = new Coordinate(*coordinate);
      transformation.apply(*new_coordinate);
      this->_windowCoordinates.push_back(new_coordinate);
    }
  }
}

void DrawableObject::updateClippingCoordinates(const Axes& axes)
{
  LOG( 8, "..." );
  this->_clippingCoordinates = this->_windowCoordinates;
}
