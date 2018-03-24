#include "viewport.h"

ViewPort::ViewPort() :
      xMin(0),
      yMin(0),
      xMax(0),
      yMax(0),
      clippingWindow("Clipping Window", std::list<Coordinate*>{})
{
}

/**
 * Prints a beauty version of the viewWindow when called on `std::cout<< viewWindow << std::end;`
 */
std::ostream& operator<<( std::ostream &output, const ViewPort &object )
{
  output
      << "(" << std::setw(4) << object.xMin << ", " << std::setw(4) << object.yMin << ")"
      << "(" << std::setw(4) << object.xMax << ", " << std::setw(4) << object.yMax << ")";
  return output;
}

ViewPort::~ViewPort()
{
}

void ViewPort::updateClippingWindowSize(int width, int height)
{
  this->_drawing_area_width  = width;
  this->_drawing_area_height = height;

  this->clippingWindow.clearCoordinates();
  this->clippingWindow.addCoordinate(new Coordinate(30        , 30         ));
  this->clippingWindow.addCoordinate(new Coordinate(30        , height - 30));
  this->clippingWindow.addCoordinate(new Coordinate(width - 30, height - 30));
  this->clippingWindow.addCoordinate(new Coordinate(width - 30, 30         ));
  this->clippingWindow.addCoordinate(new Coordinate(30        , 30         ));
}

std::list<Coordinate*> ViewPort::getCoordinates()
{
  return this->clippingWindow.getCoordinates();
}

