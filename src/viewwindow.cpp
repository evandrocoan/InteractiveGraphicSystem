#include "viewwindow.h"

ViewWindow::ViewWindow()
    : xMin(0),
      yMin(0),
      xMax(0),
      yMax(0)
{
}

/**
 * Prints a beauty version of the viewWindow when called on `std::cout<< viewWindow << std::end;`
 */
std::ostream& operator<<( std::ostream &output, const ViewWindow &object )
{
  output
      << "(" << std::setw(4) << object.xMin << ", " << std::setw(4) << object.yMin << ")"
      << "(" << std::setw(4) << object.xMax << ", " << std::setw(4) << object.yMax << ")";
  return output;
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
}

void ViewWindow::move_up(int length)
{
  this->yMin += length;
  this->yMax += length;
}

void ViewWindow::move_down(int length)
{
  this->yMin -= length;
  this->yMax -= length;
}

void ViewWindow::move_left(int length)
{
  this->xMin -= length;
  this->xMax -= length;
}

void ViewWindow::move_right(int length)
{
  this->xMin += length;
  this->xMax += length;
}

ViewWindow::~ViewWindow()
{
}
