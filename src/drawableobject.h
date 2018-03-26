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

#include "clippingwindow.h"
#include "coordinate.h"
#include "transformation.h"
#include "subject_controller.h"

class DrawableObject
{
public:
  DrawableObject(std::string name, std::list<Coordinate*> coordinates);
  ~DrawableObject();

  std::string getName();

  std::list<Coordinate*>& getCoordinates();
  std::list<Coordinate*>& getClippedCoordinates();

  /**
   * `get_geometric_center()` return a pointer which you must explicitly delete after using it.
   */
  Coordinate* get_geometric_center();

  void         apply(Transformation&);
  virtual void updateClipping(ClippingWindow&);

  /**
   * Making operator<< virtual?
   * https://stackoverflow.com/questions/4571611/making-operator-virtual
   */
  friend std::ostream& operator<<(std::ostream &output, const DrawableObject &object);
  virtual void printMyself(std::ostream &output) const;

protected:
  DrawableObject(std::string name);

  std::string name;

  std::list<Coordinate*> coordinates;
  std::list<Coordinate*> clipped_coordinates;
};
#endif // GTKMM_APP_DRAWABLE_OBJECT
