#include "point.h"

Point::Point(std::string name, Coordinate* point_cord)
    : DrawableObject(name)
{
  coordinates.push_back(point_cord);
}

Point::~Point()
{
}

