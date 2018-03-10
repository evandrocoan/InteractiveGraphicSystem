#include "viewwindow.h"

Viewwindow::Viewwindow(int Xwmin, int Ywmin, int Xwmax, int Ywmax)
    : Xwmin(Xwmin),
      Ywmin(Ywmin),
      Xwmax(Xwmax),
      Ywmax(Ywmax)
{
}

void Viewwindow::zoom_in(float scale)
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
    else
    {
      this->Xwmin = newXwmin;
      this->Xwmax = newXwmax;
      this->Ywmin = newYwmin;
      this->Ywmax = newYwmax;
    }
  }
}

void Viewwindow::zoom_out(float scale)
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
    else
    {
      this->Xwmin = newXwmin;
      this->Xwmax = newXwmax;
      this->Ywmin = newYwmin;
      this->Ywmax = newYwmax;
    }
  }
}

void Viewwindow::move_up(int length)
{
  this->Ywmin += length;
  this->Ywmax += length;
}

void Viewwindow::move_down(int length)
{
  this->Ywmin -= length;
  this->Ywmax -= length;
}

void Viewwindow::move_left(int length)
{
  this->Xwmin -= length;
  this->Xwmax -= length;
}

void Viewwindow::move_right(int length)
{
  this->Xwmin += length;
  this->Xwmax += length;
}

float Viewwindow::getXwmin()
{
  return this->Xwmin;
}

float Viewwindow::getYwmin()
{
  return this->Ywmin;
}

float Viewwindow::getXwmax()
{
  return this->Xwmax;
}

float Viewwindow::getYwmax()
{
  return this->Ywmax;
}

void Viewwindow::setXwmin(float Xwmin)
{
  this->Xwmin = Xwmin;
}

void Viewwindow::setYwmin(float Ywmin)
{
  this->Ywmin = Ywmin;
}

void Viewwindow::setXwmax(float Xwmax)
{
  this->Xwmax = Xwmax;
}

void Viewwindow::setYwmax(float Ywmax)
{
  this->Ywmax = Ywmax;
}

Viewwindow::~Viewwindow()
{
}
