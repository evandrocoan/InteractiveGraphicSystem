#ifndef GTKMM_APP_VIEWPORT_OBSERVER
#define GTKMM_APP_VIEWPORT_OBSERVER

#include <list>


class ViewportObserver {
public:
  virtual void updateDropdownList() = 0;
};


class ViewPortObservers
{
public:
  ViewPortObservers();
  virtual ~ViewPortObservers();

  void notifyObservers();
  void addObserver(ViewportObserver*);

private:
  std::list<ViewportObserver*> observers;
};

#endif // GTKMM_APP_VIEWPORT_OBSERVER
