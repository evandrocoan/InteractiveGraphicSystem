#include "viewwindow.h"

ViewWindow::ViewWindow() :
      xMin(0),
      yMin(0),
      xMax(0),
      yMax(0),
      coordinate(0, 0),
      top_left(0,0),
      top_right(0,0),
      bottom_left(0,0),
      bottom_right(0,0)
{
}

ViewWindow::~ViewWindow()
{
}

std::ostream& operator<<( std::ostream &output, const ViewWindow &object )
{
  output
      << "(" << std::setw(4) << object.xMin << ", " << std::setw(4) << object.yMin << ")"
      << "(" << std::setw(4) << object.xMax << ", " << std::setw(4) << object.yMax << ")";
  return output;
}

Signal<>::Connection ViewWindow::addObserver(const Signal<>::Callback &callback)
{
  auto connection = callObservers.connect(callback);
  this->callObservers();
  return connection;
}

void ViewWindow::zoom_in(float scale)
{
  if (scale < 1)
  {
    return;
  }
  else
  {
    float width = this->xMax - this->xMin;
    float height = this->yMax - this->yMin;

    float xMinNew = this->xMin + (width - (width / scale)) / 2;
    float xMaxNew = this->xMax - (width - (width / scale)) / 2;
    float yMinNew = this->yMin + (height - (height / scale)) / 2;
    float yMaxNew = this->yMax - (height - (height/ scale)) / 2;

    if ((xMaxNew - xMinNew) < MIN_WIDTH || (yMaxNew - yMinNew) < MIN_HEIGHT)
    {
      return;
    }
    else
    {
      this->xMin = xMinNew;
      this->xMax = xMaxNew;
      this->yMin = yMinNew;
      this->yMax = yMaxNew;
    }
  }

  this->callObservers();
}

void ViewWindow::zoom_out(float scale)
{
  if (scale < 1)
  {
    return;
  }
  else
  {
    float width = this->xMax - this->xMin;
    float height = this->yMax - this->yMin;

    float xMinNew = this->xMin - ((width * scale) - width) / 2;
    float xMaxNew = this->xMax + ((width * scale) - width) / 2;
    float yMinNew = this->yMin - ((height * scale) - height) / 2;
    float yMaxNew = this->yMax + ((height * scale) - height) / 2;

    if ((xMaxNew - xMinNew) > MAX_WIDTH || (yMaxNew - yMinNew) > MAX_HEIGHT)
    {
      return;
    }
    else
    {
      this->xMin = xMinNew;
      this->xMax = xMaxNew;
      this->yMin = yMinNew;
      this->yMax = yMaxNew;
    }
  }

  this->callObservers();
}

void ViewWindow::move_up(int length)
{
  this->initPoints();

  Transformation transformation;
  Coordinate center(0,0,0);
  transformation.add_translation("windowUp",Coordinate(0,length));
  transformation.set_geometric_center(center);

  this->applyTransformation(transformation);
  this->setPoints();

  // this->yMin += length;
  // this->yMax += length;
  this->callObservers();
}

void ViewWindow::move_down(int length)
{
  this->initPoints();

  Transformation transformation;
  Coordinate center(0,0,0);
  transformation.add_translation("windowDown",Coordinate(0,-length));
  transformation.set_geometric_center(center);

  this->applyTransformation(transformation);
  this->setPoints();

  // this->yMin -= length;
  // this->yMax -= length;
  this->callObservers();
}

void ViewWindow::move_left(int length)
{
  this->initPoints();

  Transformation transformation;
  Coordinate center(0,0,0);
  transformation.add_translation("windowLeft",Coordinate(-length,0));
  transformation.set_geometric_center(center);

  this->applyTransformation(transformation);
  this->setPoints();

  // this->xMin -= length;
  // this->xMax -= length;
  this->callObservers();
}

void ViewWindow::move_right(int length)
{
  this->initPoints();

  Transformation transformation;
  Coordinate center(0,0,0);
  transformation.add_translation("windowRight",Coordinate(length,0));
  transformation.set_geometric_center(center);

  this->applyTransformation(transformation);
  this->setPoints();

  // this->xMin += length;
  // this->xMax += length;
  this->callObservers();
}

void ViewWindow::rotate_left(GTKMM_APP_MATRICES_DATATYPE angle)
{
  this->initPoints();

  Transformation transformation;
  Coordinate center(0,0,0);

  transformation.add_translation("Center with the world",Coordinate(-this->coordinate.getx(), -this->coordinate.gety()));
  transformation.add_rotation("Window Rotation", Array<3, GTKMM_APP_MATRICES_DATATYPE>{-angle, 0.0, 0.0});
  transformation.add_translation("Back to initial position",Coordinate(this->coordinate.getx(), this->coordinate.gety()));
  transformation.set_geometric_center(center);

  this->applyTransformation(transformation);
  this->setPoints();
  this->callObservers();
}

void ViewWindow::rotate_right(GTKMM_APP_MATRICES_DATATYPE angle)
{
  this->initPoints();

  Transformation transformation;
  Coordinate center(0,0,0);

  transformation.add_translation("Center with the world",Coordinate(-this->coordinate.getx(), -this->coordinate.gety()));
  transformation.add_rotation("Window Rotation", Array<3, GTKMM_APP_MATRICES_DATATYPE>{angle, 0.0, 0.0});
  transformation.add_translation("Back to initial position",Coordinate(this->coordinate.getx(), this->coordinate.gety()));
  transformation.set_geometric_center(center);

  this->applyTransformation(transformation);
  this->setPoints();
  this->callObservers();
}

Coordinate& ViewWindow::getCoordinate()
{
  return this->coordinate;
}

void ViewWindow::setCoordinate(Coordinate coordinate)
{
  this->coordinate = coordinate;
}

void ViewWindow::initPoints()
{
  this->top_left = Coordinate(this->xMin, this->yMax);
  this->top_right = Coordinate(this->xMax, this->yMax);
  this->bottom_left = Coordinate(this->xMin, this->yMin);
  this->bottom_right = Coordinate(this->xMax, this->yMin);
}

void ViewWindow::setPoints()
{
  this->xMin = this->bottom_left.getx();
  this->yMin = this->bottom_left.gety();

  this->xMax = this->top_right.getx();
  this->yMax = this->top_right.gety();
}

void ViewWindow::applyTransformation(Transformation& transformation)
{
  transformation.apply(this->bottom_left);
  transformation.apply(this->bottom_right);
  transformation.apply(this->top_left);
  transformation.apply(this->top_right);
  transformation.apply(this->coordinate);
}





