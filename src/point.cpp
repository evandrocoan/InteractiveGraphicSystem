#include "point.h"

Point::Point(string name, Coordinate* point_cord)
    : DrawableObject(name)
{
  coordinates.push_back(point_cord);
}

Point::~Point()
{
}
