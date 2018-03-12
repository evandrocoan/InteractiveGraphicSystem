#ifndef GTKMM_APP_VIEWPORT_OBSERVER
#define GTKMM_APP_VIEWPORT_OBSERVER

#include <list>


class ViewPortObserver {
public:
  virtual void updateDropdownList() = 0;
};


class ViewPortObservers
{
public:
  ViewPortObservers();
  virtual ~ViewPortObservers();

  void notifyObservers();
  void addObserver(ViewPortObserver*);

private:
  std::list<ViewPortObserver*> observers;
};

#endif // GTKMM_APP_VIEWPORT_OBSERVER
