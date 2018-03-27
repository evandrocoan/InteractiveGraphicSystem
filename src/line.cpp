#include "line.h"

Line::Line(std::string name, Coordinate* line_cord1, Coordinate* line_cord2, Coordinate* line_cord1_in_window, Coordinate* line_cord2_in_window)
    : DrawableObject(name)
{
  coordinates.push_back(line_cord1);
  coordinates.push_back(line_cord2);
  viewWindowCoordinates.push_back(line_cord1_in_window);
  viewWindowCoordinates.push_back(line_cord2_in_window);
}

Line::~Line()
{
}
