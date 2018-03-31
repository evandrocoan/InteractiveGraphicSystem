#include "viewwindow.h"

ViewWindow::ViewWindow() :
      _width(100),
      _height(100),
      _angle(0, 0),
      _windowCenter(100, 100)
{
}

ViewWindow::~ViewWindow()
{
}

long double ViewWindow::width()
{
  return this->_width;
}

long double ViewWindow::height()
{
  return this->_height;
}

std::ostream& operator<<( std::ostream &output, const ViewWindow &object )
{
  output << "ViewWindow[" << object._width << "x" << object._height << "]"
      << object._windowCenter << " " << object._transformation;
  return output;
}

Signal<>::Connection ViewWindow::addObserver(const Signal<>::Callback &callback)
{
  auto connection = this->_observers.connect(callback);
  this->callObservers();
  return connection;
}

void ViewWindow::zoom(long int step)
{
  if( (this->_width + step <= MINIMUM_ZOOM_LIMIT || this->_height + step <= MINIMUM_ZOOM_LIMIT) && step < 0 )
  {
    LOG(1, "");
    LOG(1, "");
    LOG(1, "ERROR: You reached the maximum zoom limit! %s", *this);
    return;
  }
  else if( (this->_width + step >= MAXIMUM_ZOOM_LIMIT || this->_height + step >= MAXIMUM_ZOOM_LIMIT) && step > 0)
  {
    LOG(1, "");
    LOG(1, "");
    LOG(1, "ERROR: You reached the minimum zoom limit! %s", *this);
    return;
  }
  else
  {
    this->_width += step;
    this->_height += step;
    this->callObservers();
  }
}

void ViewWindow::move(long int horizontal_step, long int vertical_step)
{
  this->_windowCenter[0] += horizontal_step;
  this->_windowCenter[1] += vertical_step;
  this->callObservers();
}

void ViewWindow::rotate(Coordinate _angle)
{
  this->_angle += -_angle;
  this->callObservers();
}

void ViewWindow::callObservers()
{
  this->_transformation.clear();
  this->_transformation.add_translation("Translation to wold center", -this->_windowCenter);
  this->_transformation.add_rotation("Rotation on given coordinate", this->_angle);
  this->_transformation.add_scaling("Scaling for window coordinates", Coordinate(1.0/this->_width, 1.0/this->_height, 1.0));
  this->_transformation.set_geometric_center();
  LOG(4, "_width: %s", this->_width);
  LOG(4, "1/this->_width: %s", 1.0/this->_width);
  LOG(4, "_transformation: %s", _transformation);
  this->_observers();
}

void ViewWindow::apply(Coordinate& coordinate)
{
  this->_transformation.apply(coordinate);
}
