/*
 * drawableobject.h
 *
 *  Created on: 11 de mar de 2017
 *      Author: karla
 */

#ifndef GTKMM_APP_DRAWABLE_OBJECT
#define GTKMM_APP_DRAWABLE_OBJECT

 /*This class represent's a object such as line or polygon
 * to be drawed on the view port.
 * Include any library as need to use other's components.
 */
#include <list>
#include <string>
#include "coordinate.h"
using namespace std;

class DrawableObject
{
public:
  DrawableObject(string name, list<Coordinate*> coordinates) :
      name(name), coordinates(coordinates) {}

  ~DrawableObject() {}

  std::string getName()
  {
    return name;
  }

  virtual list<Coordinate*> getCoordinates() = 0;

protected:
  DrawableObject(string name) :
      name(name) {}

  string name;
  list<Coordinate*> coordinates;
};
#endif // GTKMM_APP_DRAWABLE_OBJECT
