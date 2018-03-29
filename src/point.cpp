#include "point.h"

Point::Point(std::string name, Coordinate* point_cord) :
      DrawableObject(name, std::list<Coordinate*>{point_cord})
{
}

Point::~Point()
{
}

