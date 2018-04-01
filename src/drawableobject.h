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
#include "viewwindow.h"
#include "coordinate.h"
#include "transformation.h"

class DrawableObject
{
public:
  virtual ~DrawableObject();
  std::string getName();

  void addWorldCoordinate(Coordinate* coordinate);
  void clearWorldCoordinates();

  std::list<Coordinate*>& getWorldCoordinates();
  std::list<Coordinate*>& getViewWindowCoordinates();
  std::list<Coordinate*>& getClippingCoordinates();

  static Coordinate getGeometricCenter(const std::list<Coordinate*>&);
  static void destroyList(std::list<Coordinate*>& coordinates);

  void apply(Transformation&);
  virtual void updateWindowCoordinates(const Transformation&);
  virtual void updateClippingCoordinates(ViewPort&);

  /**
   * Making operator<< virtual?
   * https://stackoverflow.com/questions/4571611/making-operator-virtual
   */
  friend std::ostream& operator<<(std::ostream &output, const DrawableObject &object);
  virtual void printMyself(std::ostream &output) const;

protected:
  DrawableObject(std::string name);
  DrawableObject(std::string name, std::list<Coordinate*> worldCoordinates);

  std::string name;

  std::list<Coordinate*> worldCoordinates;
  std::list<Coordinate*> viewWindowCoordinates;
  std::list<Coordinate*> clippingCoordinates;
};
#endif // GTKMM_APP_DRAWABLE_OBJECT
