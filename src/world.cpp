#include "world.h"

World::World()
{
  draw_xy_axes();
  // this->_displayFile.push_back(this->_points);
  // this->_displayFile.push_back(this->_lines);
  // this->_displayFile.push_back(this->_polygons);
}

World::~World()
{
}

void World::addLine(std::string name, int x1_cord, int y1_cord, int x2_cord, int y2_cord,
                    Coordinate _borderColor, LineClippingType type, bool visible_on_gui)
{
  Coordinate* point_cord1 = new Coordinate(x1_cord, y1_cord);
  Coordinate* point_cord2 = new Coordinate(x2_cord, y2_cord);

  Line* line = new Line(name, point_cord1, point_cord2, _borderColor, type, visible_on_gui);

  this->_displayFile.addObject(line);
  this->_updateObjectCoordinates(line);
}

void World::addPoint(std::string name, int x_coord, int y_coord, Coordinate _borderColor)
{
  Coordinate* point_cord = new Coordinate(x_coord, y_coord);
  Point* point = new Point(name, point_cord, _borderColor);

  this->_displayFile.addObject(point);
  this->_updateObjectCoordinates(point);
}

void World::addPolygon(std::string name, std::vector<big_double> polygon_coord_list,
      Coordinate _borderColor, Coordinate  _fillingColor, CurveType type)
{
  int unsigned coordinates_size = polygon_coord_list.size();
  std::vector<Coordinate*> coordinates;

  for( unsigned int index = 2; index < coordinates_size; index++, index++, index++ )
  {
    LOG(1, "Currently we do not support 3D, forcing z `%s` to be 1", polygon_coord_list.at(index));
    coordinates.push_back( new Coordinate( polygon_coord_list.at(index-2), polygon_coord_list.at(index-1), 1 ) );
  }

  DrawableObject* object;

  switch(type) {
    case CurveType::POLYGON:
    {
      object = new Polygon(name, coordinates, _borderColor, _fillingColor);
      break;
    }
    case CurveType::BEZIER:
    {
      object = new Curve(name, coordinates, _borderColor, _fillingColor, BEZIER);
      break;
    }
    case CurveType::BSPLINE:
    {
      object = new Curve(name, coordinates, _borderColor, _fillingColor, BSPLINE);
      break;
    }
    case CurveType::NOCURVE:
    {
      std::string error = tfm::format( "You cannot create a Polygon with %s.", type );
      throw std::runtime_error( error );
    }
  }

  this->_displayFile.addObject(object);
  this->_updateObjectCoordinates(object);
}

void World::removeObject(std::string name)
{
  // LOG(4, "Removing an object by name is faster than by pointer because it internally calls `removeObjectByName()`");
  this->_displayFile.removeObjectByName(name);
  this->_updateObjectCoordinates(nullptr);
}

World::UpdateObjectCoordinates::Connection World::addObserver(const World::UpdateObjectCoordinates::Callback& callback)
{
  return this->_updateObjectCoordinates.connect(callback);
}

void World::updateAllObjectCoordinates(const Transformation& transformation, const Axes& axes)
{
  LOG(4, "\n");
  LOG(4, "Updating all objects clipping...");
  auto objects = this->_displayFile.getObjects();

  for (auto object : objects)
  {
    object->updateWindowCoordinates(transformation);
    object->updateClippingCoordinates(axes);
  }
}

void World::updateObjectCoordinates(DrawableObject* object, const Transformation& transformation, const Axes& axes)
{
  if( object == nullptr )
  {
    LOG(1, "");
    LOG(1, "");
    LOG(1, "ERROR: Null pointer object passed by: %s", object);
    return;
  }

  if( object->worldCoordinates().size() == 0 )
  {
    LOG(1, "");
    LOG(1, "");
    LOG(1, "ERROR: The object `%s` has no coordinates.", *object);
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
      DrawableObject* object = this->_displayFile.apply(object_name, transformation);
      this->_updateObjectCoordinates(object);
    }
    else
    {
      LOG(4, "There are no transformations available to be applied on your object: `%s` %s", object_name, transformation);
    }
  }
  else
  {
    LOG(4, "No object was found within the name: `%s`", object_name);
  }
}

void World::draw_xy_axes()
{
  LOG(4, "Drawing the X T axes as world objects.");
  this->addLine("Y Axe", 0, -WORLD_AXES_SIZE, 0, WORLD_AXES_SIZE, Coordinate(0.741176, 0.717647, 0.419608), LIANG_BARSKY, false);
  this->addLine("X Axe", -WORLD_AXES_SIZE, 0, WORLD_AXES_SIZE, 0, Coordinate(0.741176, 0.717647, 0.419608), LIANG_BARSKY, false);
}
