#include "line.h"

Line::Line(std::string name, Coordinate* line_cord1, Coordinate* line_cord2)
    : DrawableObject(name)
{
  coordinates.push_back(line_cord1);
  coordinates.push_back(line_cord2);
}

Line::~Line()
{
}
