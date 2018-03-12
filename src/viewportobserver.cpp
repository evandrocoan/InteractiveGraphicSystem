#include "viewportobserver.h"

ViewportObservers::ViewportObservers()
{
}

ViewportObservers::~ViewportObservers()
{
}

void ViewportObservers::addObserver(ViewportObserver* observer)
{
  this->observers.push_back(observer);
}

void ViewportObservers::notifyObservers()
{
  for( ViewportObserver* observer : this->observers )
  {
    observer->updateDropdownList();
  }
}

