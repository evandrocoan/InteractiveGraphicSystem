#include "point.h"

Point::Point(std::string name, Coordinate* point_cord, Coordinate* point_cord_in_window)
    : DrawableObject(name)
{
  coordinates.push_back(point_cord);
  viewWindowCoordinates.push_back(point_cord_in_window);
}

Point::~Point()
{
}
