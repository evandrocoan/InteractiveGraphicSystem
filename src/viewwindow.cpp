#include "viewwindow.h"

ViewWindow::ViewWindow(int xWmin, int yWmin, int xWmax, int yWmax)
    : xWmin(xWmin),
      yWmin(yWmin),
      xWmax(xWmax),
      yWmax(yWmax),
      coordinate(xWmax/2,yWmax/2),
      top_left(0,0),
      top_right(0,0),
      bottom_left(0,0),
      bottom_right(0,0)
{
}

void ViewWindow::zoom_in(float scale)
{
  if (scale < 1)
  {
    return;
  }
  else
  {
    float width = this->xWmax - this->xWmin;
    float height = this->yWmax - this->yWmin;

    float xWminNew = this->xWmin + (width - (width / scale)) / 2;
    float xWmaxNew = this->xWmax - (width - (width / scale)) / 2;
    float yWminNew = this->yWmin + (height - (height / scale)) / 2;
    float yWmaxNew = this->yWmax - (height - (height/ scale)) / 2;

    if ((xWmaxNew - xWminNew) < MIN_WIDTH || (yWmaxNew - yWminNew) < MIN_HEIGHT)
    {
      return;
    }
    else
    {
      this->xWmin = xWminNew;
      this->xWmax = xWmaxNew;
      this->yWmin = yWminNew;
      this->yWmax = yWmaxNew;
    }
  }
}

void ViewWindow::zoom_out(float scale)
{
  if (scale < 1)
  {
    return;
  }
  else
  {
    float width = this->xWmax - this->xWmin;
    float height = this->yWmax - this->yWmin;

    float xWminNew = this->xWmin - ((width * scale) - width) / 2;
    float xWmaxNew = this->xWmax + ((width * scale) - width) / 2;
    float yWminNew = this->yWmin - ((height * scale) - height) / 2;
    float yWmaxNew = this->yWmax + ((height * scale) - height) / 2;

    if ((xWmaxNew - xWminNew) > MAX_WIDTH || (yWmaxNew - yWminNew) > MAX_HEIGHT)
    {
      return;
    }
    else
    {
      this->xWmin = xWminNew;
      this->xWmax = xWmaxNew;
      this->yWmin = yWminNew;
      this->yWmax = yWmaxNew;
    }
  }
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
}

void ViewWindow::rotate_left(long double angle)
{
  this->initPoints();

  Transformation transformation;
  Coordinate center(0,0,0);
  
  transformation.add_translation("Center with the world",Coordinate(-this->coordinate.getx(), -this->coordinate.gety()));
  transformation.add_rotation("Window Rotation", Array<3, long double>{-angle, 0.0, 0.0});
  transformation.add_translation("Back to initial position",Coordinate(this->coordinate.getx(), this->coordinate.gety()));
  transformation.set_geometric_center(center);

  this->applyTransformation(transformation);

  this->setPoints();

  
}

void ViewWindow::rotate_right(long double angle)
{

  this->initPoints();

  Transformation transformation;
  Coordinate center(0,0,0);
  
  transformation.add_translation("Center with the world",Coordinate(-this->coordinate.getx(), -this->coordinate.gety()));
  transformation.add_rotation("Window Rotation", Array<3, long double>{angle, 0.0, 0.0});
  transformation.add_translation("Back to initial position",Coordinate(this->coordinate.getx(), this->coordinate.gety()));
  transformation.set_geometric_center(center);

  this->applyTransformation(transformation);

  this->setPoints();
  
}

void ViewWindow::move_center(){

  this->initPoints();

  Transformation transformation;
  Coordinate center(0,0,0);
  
  transformation.add_translation("Center with the world",Coordinate(-this->coordinate.getx(), -this->coordinate.gety()));
  transformation.set_geometric_center(this->coordinate);

  this->applyTransformation(transformation);

  this->setPoints();
 


}

ViewWindow::~ViewWindow()
{
}

Coordinate ViewWindow::getCoordinate()
{
  return this->coordinate;
}

void ViewWindow::setCoordinate(Coordinate coordinate)
{
  this->coordinate = coordinate;
}

void ViewWindow::initPoints(){

  this->top_left = Coordinate(this->xWmin, this->yWmax);
  this->top_right = Coordinate(this->xWmax, this->yWmax);
  this->bottom_left = Coordinate(this->xWmin, this->yWmin);
  this->bottom_right = Coordinate(this->xWmax, this->yWmin);

}

void ViewWindow::setPoints(){

  this->xWmin = this->bottom_left.getx();
  this->yWmin = this->bottom_left.gety();

  this->xWmax = this->top_right.getx();
  this->yWmax = this->top_right.gety();

}

void ViewWindow::applyTransformation(Transformation transformation){

  transformation.apply(this->bottom_left);
  transformation.apply(this->bottom_right);
  transformation.apply(this->top_left);
  transformation.apply(this->top_right);
  transformation.apply(this->coordinate);

}





