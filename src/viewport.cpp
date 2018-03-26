#include "viewport.h"

ViewPort::ViewPort() :
      xMin(0),
      yMin(0),
      xMax(0),
      yMax(0),
      axes()
{
}

/**
 * Prints a beauty version of the viewWindow when called on `std::cout<< viewWindow << std::end;`
 */
std::ostream& operator<<( std::ostream &output, const ViewPort &object )
{
  output
      << "(" << std::setw(4) << object.xMin << ", " << std::setw(4) << object.yMin << ")"
      << "(" << std::setw(4) << object.xMax << ", " << std::setw(4) << object.yMax << ") ";
  return output;
}

ViewPort::~ViewPort()
{
}

/**
 * Accessing the [] operator from a pointer
 * https://stackoverflow.com/questions/8493829/accessing-the-operator-from-a-pointer
 */
void ViewPort::updateClippingWindowSize(int width, int height)
{
  this->axes.updateClippingWindowSize(width, height);
}

ClippingWindow& ViewPort::getCoordinates()
{
  return this->axes;
}

