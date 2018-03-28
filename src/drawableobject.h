/*
 * drawableobject.h
 *
 *  Created on: 11 de mar de 2017
 *      Author: karla
 */

#ifndef GTKMM_APP_DRAWABLE_OBJECT
#define GTKMM_APP_DRAWABLE_OBJECT

/**
 * This class represents a object such as line or polygon to be draw on the viewport.
 */
#include <list>
#include <string>

#include "debugger.h"

#include "viewport.h"
#include "coordinate.h"
#include "transformation.h"

class DrawableObject
{
public:
  virtual ~DrawableObject();
  std::string getName();

  std::list<Coordinate*>& getCoordinates();
  std::list<Coordinate*>& getviewWindowCoordinates();

  void setviewWindowCoordinates(std::list<Coordinate*> coordinates);
  void setCoordinates(std::list<Coordinate*> coordinates);

  std::list<Coordinate*>& getClippedCoordinates();

  /**
   * `get_geometric_center()` return a pointer which you must explicitly delete after using it.
   */
  Coordinate* get_geometric_center();
  Coordinate* get_window_geometric_center();

  void apply(Transformation&);
  void applyInWindow(Transformation&);

  virtual void updateClipping(ViewPort&);

  /**
   * Making operator<< virtual?
   * https://stackoverflow.com/questions/4571611/making-operator-virtual
   */
  friend std::ostream& operator<<(std::ostream &output, const DrawableObject &object);
  virtual void printMyself(std::ostream &output) const;

protected:
  DrawableObject(std::string name);
  DrawableObject(std::string name, std::list<Coordinate*> coordinates);

  std::string name;

  std::list<Coordinate*> coordinates;
  std::list<Coordinate*> viewWindowCoordinates;
  std::list<Coordinate*> clipped_coordinates;

private:
  void destroyList(std::list<Coordinate*> coordinates);
};
#endif // GTKMM_APP_DRAWABLE_OBJECT
