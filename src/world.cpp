#include "world.h"

World::World()
{
  draw_xy_axes();
}

World::~World()
{
}

void World::addLine(std::string name, int x1, int y1, int z1, int x2, int y2, int z2,
                    Coordinate _borderColor, LineClippingType type, bool _isVisibleOnGui)
{
  Coordinate* point1 = new Coordinate(x1, y1, z1);
  Coordinate* point2 = new Coordinate(x2, y2, z2);

  Line* line = new Line(name, point1, point2, _borderColor, type, _isVisibleOnGui);

  this->_lines.addObject(line);
  this->_displayFile.addObject(line);
  this->_updateObjectCoordinates(line);
}

void World::addPoint(std::string name, int x, int y, int z, Coordinate _borderColor)
{
  Coordinate* point_coordinate = new Coordinate(x, y, z);
  Point* point = new Point(name, point_coordinate, _borderColor);

  this->_points.addObject(point);
  this->_displayFile.addObject(point);
  this->_updateObjectCoordinates(point);
}

void World::addPolygon(std::string name, std::vector<big_double> coordinates_points,
      Coordinate _borderColor, Coordinate  _fillingColor, CurveType type)
{
  int unsigned coordinates_size = coordinates_points.size();
  std::vector<Coordinate*> coordinates;

  for( unsigned int index = 2; index < coordinates_size; index++, index++, index++ )
  {
    coordinates.push_back( new Coordinate( coordinates_points.at(index-2), coordinates_points.at(index-1), coordinates_points.at(index) ) );
  }

  this->addPolygon( name, coordinates, _borderColor, _fillingColor, type );
}

void World::addPolygon(std::string name, std::vector<Coordinate*> coordinates,
      Coordinate _borderColor, Coordinate  _fillingColor, CurveType type)
{
  DrawableObject* object;

  switch(type) {
    case CurveType::POLYGON:
    {
      Polygon* polygon = new Polygon(name, coordinates, _borderColor, _fillingColor);
      object = polygon;

      this->_polygons.addObject(polygon);
      break;
    }
    case CurveType::BEZIER:
    {
      Curve* curve = new BezierCurve(name, coordinates, _borderColor, _fillingColor);
      object = curve;

      this->_curves.addObject(curve);
      break;
    }
    case CurveType::BSPLINE:
    {
      Curve* curve = new BsplineCurve(name, coordinates, _borderColor, _fillingColor);
      object = curve;

      this->_curves.addObject(curve);
      break;
    }
    case CurveType::NOCURVE: {
      std::string error = tfm::format( "You cannot create a Polygon with type: %s.", type );

      LOG( 1, "%s", error );
      throw std::runtime_error( error );
    }
  }

  this->_displayFile.addObject(object);
  this->_updateObjectCoordinates(object);
}

void World::addPolyhedron(std::string name, std::vector<Coordinate*> points,
      std::vector<int> _line_segments, int _facet_size,
      Coordinate _borderColor, Coordinate _fillingColor)
{
  Polyhedron* polyhedron = new Polyhedron(name, points, _line_segments, _facet_size, _borderColor, _fillingColor);
  this->_polyhedrons.addObject(polyhedron);

  this->_displayFile.addObject(polyhedron);
  this->_updateObjectCoordinates(polyhedron);
}

void World::setLineClipping(LineClippingType type)
{
  auto objects = this->_lines.getObjects();

  for( auto line : objects )
  {
    line->setLineClipping( type );
  }
}

void World::removeObject(std::string name)
{
  // LOG(4, "Removing an object by name is faster than by pointer because it internally calls `removeObjectByName()`");

  if( this->_displayFile.isObjectOnByName(name) )
  {
    if( this->_polygons.isObjectOnByName(name) )
    {
      this->_polygons.removeObjectByName(name);
    }
    else if( this->_polyhedrons.isObjectOnByName(name) )
    {
      this->_polyhedrons.removeObjectByName(name);
    }
    else if( this->_curves.isObjectOnByName(name) )
    {
      this->_curves.removeObjectByName(name);
    }
    else if( this->_lines.isObjectOnByName(name) )
    {
      this->_lines.removeObjectByName(name);
    }
    else if( this->_points.isObjectOnByName(name) )
    {
      this->_points.removeObjectByName(name);
    }
    else {
      std::string error = tfm::format( "The object is not found: `%s`", name );

      LOG( 1, "%s", error );
      throw std::runtime_error( error );
    }
  }
  else {
    std::string error = tfm::format( "The object is not found: `%s`", name );

    LOG( 1, "%s", error );
    throw std::runtime_error( error );
  }

  this->_displayFile.removeObjectByName(name);
  this->_updateObjectCoordinates(nullptr);
}

World::UpdateObjectCoordinates::Connection World::addObserver(const World::UpdateObjectCoordinates::Callback& callback)
{
  return this->_updateObjectCoordinates.connect(callback);
}

void World::updateAllObjectCoordinates(const Transformation& transformation, const Axes& axes)
{
  LOGLN( 8, "\n" );
  LOG( 8, "..." );
  auto objects = this->_displayFile.getObjects();

  for (auto object : objects)
  {
    object->updateWindowCoordinates(transformation);
    object->updateClippingCoordinates(axes);
  }
}

void World::updateObjectCoordinates(DrawableObject* object, const Transformation& transformation, const Axes& axes)
{
  if( object == nullptr ) {
    std::string error = tfm::format( "ERROR: Null pointer object passed by: %s", object );

    LOG( 1, "%s", error );
    throw std::runtime_error( error );
  }

  if( object->worldCoordinates().size() == 0 ) {
    std::string error = tfm::format( "ERROR: The object `%s` has no coordinates.", *object );

    LOG( 1, "%s", error );
    throw std::runtime_error( error );
  }
  else
  {
    LOG(4, "Updating the object %s with %s", *object, transformation);
  }

  object->updateWindowCoordinates(transformation);
  object->updateClippingCoordinates(axes);
}

void World::apply(const std::string object_name, Transformation &transformation)
{
  if( this->_displayFile.isObjectOnByName(object_name) )
  {
    if( transformation.size() )
    {
      DrawableObject* object = this->_displayFile.apply(object_name, transformation);
      this->_updateObjectCoordinates(object);
    }
    else {
      std::string error = tfm::format(
          "There are no transformations available to be applied on your object: `%s` %s", object_name, transformation );

      LOG( 1, "%s", error );
      throw std::runtime_error( error );
    }
  }
  else {
    std::string error = tfm::format( "No object was found within the name: `%s`", object_name );

    LOG( 1, "%s", error );
    throw std::runtime_error( error );
  }
}

void World::draw_xy_axes()
{
  LOG(4, "Drawing the X T axes as world objects.");
  constexpr int AXES_SIZE = 500;

  this->addLine("Y Axe",
      0, -AXES_SIZE, 0,
      0,  AXES_SIZE, 0,
      Coordinate(0.741176, 0.717647, 0.419608), LIANG_BARSKY, false);

  this->addLine("X Axe",
      -AXES_SIZE, 0, 0,
       AXES_SIZE, 0, 0,
      Coordinate(0.741176, 0.717647, 0.419608), LIANG_BARSKY, false);

  this->addLine("Z Axe",
      0, 0, -AXES_SIZE,
      0, 0,  AXES_SIZE,
      Coordinate(0.741176, 0.717647, 0.419608), LIANG_BARSKY, false);
}
