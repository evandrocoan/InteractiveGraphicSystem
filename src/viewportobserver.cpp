#include "viewportobserver.h"

ViewPortObservers::ViewPortObservers()
{
}

ViewPortObservers::~ViewPortObservers()
{
}

void ViewPortObservers::addObserver(ViewportObserver* observer)
{
  this->observers.push_back(observer);
}

void ViewPortObservers::notifyObservers()
{
  for( ViewportObserver* observer : this->observers )
  {
    observer->updateDropdownList();
  }
}

