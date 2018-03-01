#include "viewwindow.h"

ViewWindow::ViewWindow(int Xwmin, int Ywmin, int Xwmax, int Ywmax)
    : Xwmin(Xwmin),
      Ywmin(Ywmin),
      Xwmax(Xwmax),
      Ywmax(Ywmax)
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
    float width = this->Xwmax - this->Xwmin;
    float height = this->Ywmax - this->Ywmin;
    float newXwmin = Xwmin + (width - (width / scale)) / 2;
    float newXwmax = Xwmax - (width - (width / scale)) / 2;
    float newYwmin = Ywmin + (height - (height / scale)) / 2;
    float newYwmax = Ywmax - (height - (height/ scale)) / 2;
    if ((newXwmax - newXwmin) < MIN_WIDTH || (newYwmax - newYwmin) < MIN_HEIGHT)
    {
      return;
    }
    else{
      this->Xwmin = newXwmin;
      this->Xwmax = newXwmax;
      this->Ywmin = newYwmin;
      this->Ywmax = newYwmax;
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
    float width = this->Xwmax - this->Xwmin;
    float height = this->Ywmax - this->Ywmin;
    float newXwmin = Xwmin - ((width * scale) - width) / 2;
    float newXwmax = Xwmax + ((width * scale) - width) / 2;
    float newYwmin = Ywmin - ((height * scale) - height) / 2;
    float newYwmax = Ywmax + ((height * scale) - height) / 2;
    if ((newXwmax - newXwmin) > MAX_WIDTH || (newYwmax - newYwmin) > MAX_HEIGHT)
    {
      return;
    }
    else{
      this->Xwmin = newXwmin;
      this->Xwmax = newXwmax;
      this->Ywmin = newYwmin;
      this->Ywmax = newYwmax;
    }
  }
}

void ViewWindow::move_up(int length)
{
  this->Ywmin += length;
  this->Ywmax += length;
}

void ViewWindow::move_down(int length)
{
  this->Ywmin -= length;
  this->Ywmax -= length;
}

void ViewWindow::move_left(int length)
{
  this->Xwmin -= length;
  this->Xwmax -= length;
}

void ViewWindow::move_right(int length)
{
  this->Xwmin += length;
  this->Xwmax += length;
}

float ViewWindow::getXwmin()
{
  return this->Xwmin;
}

float ViewWindow::getYwmin()
{
  return this->Ywmin;
}

float ViewWindow::getXwmax()
{
  return this->Xwmax;
}

float ViewWindow::getYwmax()
{
  return this->Ywmax;
}

void ViewWindow::setXwmin(float Xwmin)
{
  this->Xwmin = Xwmin;
}

void ViewWindow::setYwmin(float Ywmin)
{
  this->Ywmin = Ywmin;
}

void ViewWindow::setXwmax(float Xwmax)
{
  this->Xwmax = Xwmax;
}

void ViewWindow::setYwmax(float Ywmax)
{
  this->Ywmax = Ywmax;
}

DisplayFile* ViewWindow::getDisplayFile()
{
  return &this->displayFile;
}

ViewWindow::~ViewWindow()
{
}
