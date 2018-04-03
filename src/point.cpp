#include "point.h"

Point::Point(std::string name, Coordinate* point_cord) :
      DrawableObject(name, std::list<Coordinate*>{point_cord})
{
}

Point::~Point()
{
}

void Point::updateClippingCoordinates(const Axes& axes)
{
  LOG(4, "Point clipping update... %s", axes);
  this->destroyList(this->clippingCoordinates);

  for( auto coordinate : this->windowCoordinates )
  {
    this->clippingCoordinates.push_back(new Coordinate(*coordinate));
  }

  auto& c = **this->clippingCoordinates.begin();
  this->_isDrawable = ( c.x >= axes.xWiMin && c.x <= axes.xWiMax && c.y >= axes.yWiMin && c.y <= axes.yWiMax );
}
