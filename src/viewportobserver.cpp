#include "viewportobserver.h"

ViewPortObservers::ViewPortObservers()
{
}

ViewPortObservers::~ViewPortObservers()
{
}

void ViewPortObservers::addObserver(ViewPortObserver* observer)
{
  this->observers.push_back(observer);
}

void ViewPortObservers::notifyObservers()
{
  for( ViewPortObserver* observer : this->observers )
  {
    observer->updateDropdownList();
  }
}

