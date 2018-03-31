#include "viewwindow.h"

ViewWindow::ViewWindow() :
      _angles(0, 0),
      _dimentions(100, 100),
      _windowCenter(100, 100)
{
}

ViewWindow::~ViewWindow()
{
}

long double ViewWindow::width()
{
  return this->_dimentions[0];
}

long double ViewWindow::height()
{
  return this->_dimentions[1];
}

std::ostream& operator<<( std::ostream &output, const ViewWindow &object )
{
  output << "ViewWindow[" << object._dimentions << "]"
      << object._windowCenter << " " << object._transformation << " " << object._angle_rotation;
  return output;
}

Signal<>::Connection ViewWindow::addObserver(const Signal<>::Callback &callback)
{
  auto connection = this->_observers.connect(callback);
  this->callObservers();
  return connection;
}

void ViewWindow::zoom(Coordinate step)
{
  if( (this->_dimentions[0] + step[0] <= MINIMUM_ZOOM_LIMIT
      || this->_dimentions[1] + step[0] <= MINIMUM_ZOOM_LIMIT)
    && step[0] < 0 )
  {
    LOG(1, "");
    LOG(1, "");
    LOG(1, "ERROR: You reached the maximum zoom limit! %s", *this);
    return;
  }
  else if( (this->_dimentions[0] + step[0] >= MAXIMUM_ZOOM_LIMIT
            || this->_dimentions[1] + step[0] >= MAXIMUM_ZOOM_LIMIT)
          && step[0] > 0)
  {
    LOG(1, "");
    LOG(1, "");
    LOG(1, "ERROR: You reached the minimum zoom limit! %s", *this);
    return;
  }
  else
  {
    this->_dimentions += step;
    this->callObservers();
  }
}

void ViewWindow::move(Coordinate movement)
{
  this->_angle_rotation.clear();
  this->_angle_rotation.add_rotation("Transformation on the ViewWindow by Rotation", this->_angles);
  this->_angle_rotation.apply(movement);

  this->_windowCenter += movement;
  this->callObservers();
}

void ViewWindow::rotate(Coordinate angles)
{
  this->_angles += -angles;
  this->callObservers();
}

void ViewWindow::callObservers()
{
  this->_transformation.clear();
  this->_transformation.add_translation("Translation to wold center", -this->_windowCenter);
  this->_transformation.add_rotation("Rotation on given coordinate", this->_angles);
  this->_transformation.add_scaling("Scaling for window coordinates", this->_dimentions.inverse());
  this->_transformation.set_geometric_center();
  LOG(4, "_width: %s, 1/this->_width: %s", this->_dimentions, this->_dimentions.inverse());
  LOG(4, "_transformation: %s", _transformation);
  this->_observers();
}

void ViewWindow::apply(Coordinate& coordinate)
{
  this->_transformation.apply(coordinate);
}
