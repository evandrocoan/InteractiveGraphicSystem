#ifndef GTKMM_APP_VIEWPORT_OBSERVER
#define GTKMM_APP_VIEWPORT_OBSERVER

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

  void notifyObservers();
  void addObserver(ViewportObserver*);

private:
  std::list<ViewportObserver*> observers;
};

#endif // GTKMM_APP_VIEWPORT_OBSERVER
