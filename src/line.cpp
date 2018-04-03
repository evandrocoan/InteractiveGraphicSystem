#include "line.h"

Line::Line(std::string name, Coordinate* line_cord1, Coordinate* line_cord2) :
      DrawableObject(name, std::list<Coordinate*>{line_cord1, line_cord2})
{
}

Line::~Line()
{
}

void Line::updateClippingCoordinates(const Axes& axes)
{
  LOG(4, "Line clipping update... %s", axes);
  this->clippingCoordinates = this->worldCoordinates;
}
