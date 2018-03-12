#ifndef GTKMM_APP_VIEWPORT_OBSERVERS
#define GTKMM_APP_VIEWPORT_OBSERVERS

#include <list>


class ViewportObserver {
public:
  virtual void updateDropdownList() = 0;
};


class ViewportObservers
{
public:
  ViewportObservers();
  virtual ~ViewportObservers();

  void addObserver(ViewportObserver*);
  void notifyObservers();

private:
  std::list<ViewportObserver*> observers;
};

#endif // GTKMM_APP_VIEWPORT_OBSERVERS
