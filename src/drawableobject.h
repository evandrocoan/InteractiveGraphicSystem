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

#include "coordinate.h"
#include "transformation.h"
#include "debugger.h"

class DrawableObject
{
public:
  DrawableObject(std::string name, std::list<Coordinate*> coordinates, std::list<Coordinate*> coordinates_in_window);
  DrawableObject(std::string name);
  DrawableObject(std::string name, std::list<Coordinate*> coordinates);

  ~DrawableObject();

  std::string getName();
  std::list<Coordinate*>& getCoordinates();
  std::list<Coordinate*>& getviewWindowCoordinates();

  void setCoordinates(std::list<Coordinate*> coordinates);

  /**
   * `get_geometric_center()` return a pointer which you must explicitly delete after using it.
   */
  void        apply(Transformation&);
  void        applyInWindow(Transformation&);
  Coordinate* get_geometric_center();
  Coordinate* get_window_geometric_center();

  friend std::ostream& operator<<(std::ostream &output, const DrawableObject &object);

protected:

  std::string name;
  std::list<Coordinate*> coordinates;
  std::list<Coordinate*> viewWindowCoordinates;

private:
  void destroyList(std::list<Coordinate*> coordinates);
};
#endif // GTKMM_APP_DRAWABLE_OBJECT
