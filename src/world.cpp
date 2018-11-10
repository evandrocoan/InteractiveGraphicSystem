#include "world.h"

World::World()
{
  draw_xy_axes();
}

World::~World()
{
}

void World::addLine(std::string name, int x1_cord, int y1_cord, int x2_cord, int y2_cord,
                    Coordinate _borderColor, LineClippingType type, bool _isVisibleOnGui)
{
  Coordinate* point_cord1 = new Coordinate(x1_cord, y1_cord);
  Coordinate* point_cord2 = new Coordinate(x2_cord, y2_cord);

  Line* line = new Line(name, point_cord1, point_cord2, _borderColor, type, _isVisibleOnGui);

  this->_polygons.addObject(line);
  this->_displayFile.addObject(line);
  this->_updateObjectCoordinates(line);
}

void World::addPoint(std::string name, int x_coord, int y_coord, Coordinate _borderColor)
{
  Coordinate* point_cord = new Coordinate(x_coord, y_coord);
  Point* point = new Point(name, point_cord, _borderColor);

  this->_polygons.addObject(point);
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
    LOG(1, "Currently we do not support 3D, forcing z `%s` to be 1", coordinates_points.at(index));
    coordinates.push_back( new Coordinate( coordinates_points.at(index-2), coordinates_points.at(index-1), 1 ) );
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
      object = new Polygon(name, coordinates, _borderColor, _fillingColor);
      this->_polygons.addObject(object);
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
      // object = new BsplineCurve(name, coordinates, _borderColor, _fillingColor);
      // object = curve;

      // this->_curves.addObject(curve);
      break;
    }
    case CurveType::NOCURVE: {
      std::string error = tfm::format( "You cannot create a Polygon with %s.", type );

      LOG( 1, "%s", error );
      throw std::runtime_error( error );
    }
  }

  this->_displayFile.addObject(object);
  this->_updateObjectCoordinates(object);
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
    else if( this->_displayFile.isObjectOnByName(name) )
    {
      this->_curves.removeObjectByName(name);
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
  LOGLN(4, "\n");
  LOG(4, "...");
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
    LOG(4, "Updating the object `%s`", *object);
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
      DrawableObject* object;

      if( this->_polygons.isObjectOnByName(object_name) )
      {
        object = this->_polygons.apply(object_name, transformation);
      }
      else if( this->_curves.isObjectOnByName(object_name) )
      {
        object = this->_curves.apply(object_name, transformation);
      }
      else
      {
        std::string error = tfm::format( "Inconsistency on the system. The object is not found internally: `%s`", object_name );

        LOG( 1, "%s", error );
        throw std::runtime_error( error );
      }

      this->_updateObjectCoordinates(object);
    }
    else
    {
      std::string error = tfm::format(
          "There are no transformations available to be applied on your object: `%s` %s", object_name, transformation );

      LOG( 1, "%s", error );
      throw std::runtime_error( error );
    }
  }
  else
  {
    std::string error = tfm::format( "No object was found within the name: `%s`", object_name );

    LOG( 1, "%s", error );
    throw std::runtime_error( error );
  }
}

void World::draw_xy_axes()
{
  LOG(4, "Drawing the X T axes as world objects.");
  this->addLine("Y Axe", 0, -WORLD_AXES_SIZE, 0, WORLD_AXES_SIZE, Coordinate(0.741176, 0.717647, 0.419608), LIANG_BARSKY, false);
  this->addLine("X Axe", -WORLD_AXES_SIZE, 0, WORLD_AXES_SIZE, 0, Coordinate(0.741176, 0.717647, 0.419608), LIANG_BARSKY, false);
}
