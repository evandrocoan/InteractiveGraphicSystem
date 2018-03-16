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

class DrawableObject
{
public:
  DrawableObject(std::string name, std::list<Coordinate*> coordinates);
  ~DrawableObject();

  std::string            getName();
  std::list<Coordinate*> getCoordinates();

  void       apply(Transformation);
  Coordinate get_geometric_center();

protected:
  DrawableObject(std::string name);

  std::string name;
  std::list<Coordinate*> coordinates;
};
#endif // GTKMM_APP_DRAWABLE_OBJECT
