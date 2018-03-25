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
#include "subject_controller.h"

class DrawableObject
{
public:
  DrawableObject(std::string name, std::list<Coordinate*> coordinates);
  ~DrawableObject();

  virtual void updateClipping();
  std::string getName();

  /**
   * Save the observed object disconnect callback to disconnect it from the observers list when
   * removing the object from the program by calling `disconnectObserver()`.
   */
  void addConnection(Signal<>::Connection);
  void disconnectObserver();

  std::list<Coordinate*>& getCoordinates();
  std::list<Coordinate*>& getClippedCoordinates();

  /**
   * `get_geometric_center()` return a pointer which you must explicitly delete after using it.
   */
  void        apply(Transformation&);
  Coordinate* get_geometric_center();

  friend std::ostream& operator<<(std::ostream &output, const DrawableObject &object);

protected:
  DrawableObject(std::string name);

  std::string name;
  Signal<>::Connection _connection;

  std::list<Coordinate*> coordinates;
  std::list<Coordinate*> clipped_coordinates;
};
#endif // GTKMM_APP_DRAWABLE_OBJECT
