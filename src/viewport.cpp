#include "viewport.h"

ViewPort::ViewPort() :
      xMin(0),
      yMin(0),
      xMax(0),
      yMax(0),
      _clippingWindow("Clipping Window", std::list<Coordinate*>{})
{
  LOG(4, "Random initial values which will be updated on updateClippingWindowSize()");
  int width  = 500;
  int height = 500;

  this->_firstCoordinate  = new Coordinate(clipping_window_margin_distance        , clipping_window_margin_distance         );
  this->_secondCoordinate = new Coordinate(clipping_window_margin_distance        , height - clipping_window_margin_distance);
  this->_thirdCoordinate  = new Coordinate(width - clipping_window_margin_distance, height - clipping_window_margin_distance);
  this->_forthCoordinate  = new Coordinate(width - clipping_window_margin_distance, clipping_window_margin_distance         );

  this->x1_axe = new Line("x1_axe", _firstCoordinate , _forthCoordinate  );
  this->x2_axe = new Line("x2_axe", _secondCoordinate, _thirdCoordinate  );
  this->y1_axe = new Line("y1_axe", _firstCoordinate , _secondCoordinate );
  this->y2_axe = new Line("y2_axe", _forthCoordinate , _thirdCoordinate  );

  this->_clippingWindow.addCoordinate(_firstCoordinate);
  this->_clippingWindow.addCoordinate(_secondCoordinate);
  this->_clippingWindow.addCoordinate(_thirdCoordinate);
  this->_clippingWindow.addCoordinate(_forthCoordinate);
  this->_clippingWindow.addCoordinate(_firstCoordinate);
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
  this->_firstCoordinate->_data[0]  = clipping_window_margin_distance;
  this->_firstCoordinate->_data[1]  = clipping_window_margin_distance;

  this->_secondCoordinate->_data[0] = clipping_window_margin_distance;
  this->_secondCoordinate->_data[1] = height - clipping_window_margin_distance;

  this->_thirdCoordinate->_data[0]  = width - clipping_window_margin_distance;
  this->_thirdCoordinate->_data[1]  = height - clipping_window_margin_distance;

  this->_forthCoordinate->_data[0]  = width - clipping_window_margin_distance;
  this->_forthCoordinate->_data[1]  = clipping_window_margin_distance;
}

std::list<Coordinate*> ViewPort::getCoordinates()
{
  return this->_clippingWindow.getCoordinates();
}

