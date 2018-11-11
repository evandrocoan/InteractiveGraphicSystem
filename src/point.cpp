#include "point.h"

Point::Point(std::string name, Coordinate* point_cord, Coordinate _borderColor) :
      DrawableObject(name, std::vector<Coordinate*>{point_cord}, _borderColor)
{
}

Point::~Point()
{
}

void Point::updateClippingCoordinates(const Axes& axes)
{
  LOG( 8, "..." );
  this->destroyList(this->_clippingCoordinates);

  for( auto coordinate : this->_windowCoordinates )
  {
    this->_clippingCoordinates.push_back(new Coordinate(*coordinate));
  }

  auto& c = **this->_clippingCoordinates.begin();
  this->_isDrawable = ( c.x >= axes.xWiMin && c.x <= axes.xWiMax && c.y >= axes.yWiMin && c.y <= axes.yWiMax );
}
