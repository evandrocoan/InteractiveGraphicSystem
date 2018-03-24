#include "viewwindow.h"

ViewWindow::ViewWindow(int xWmin, int yWmin, int xWmax, int yWmax)
    : xWmin(xWmin),
      yWmin(yWmin),
      xWmax(xWmax),
      yWmax(yWmax)
{
}

/**
 * Prints a beauty version of the viewWindow when called on `std::cout<< viewWindow << std::end;`
 */
std::ostream& operator<<( std::ostream &output, const ViewWindow &object )
{
  output << "ViewWindow"
      << "(" << std::setw(4) << object.xWmin << ", " << std::setw(4) << object.yWmin << ")"
      << "(" << std::setw(4) << object.xWmax << ", " << std::setw(4) << object.yWmax << ")";
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
  this->yWmin += length;
  this->yWmax += length;
}

void ViewWindow::move_down(int length)
{
  this->yWmin -= length;
  this->yWmax -= length;
}

void ViewWindow::move_left(int length)
{
  this->xWmin -= length;
  this->xWmax -= length;
}

void ViewWindow::move_right(int length)
{
  this->xWmin += length;
  this->xWmax += length;
}

ViewWindow::~ViewWindow()
{
}
