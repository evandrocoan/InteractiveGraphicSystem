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

#include "axes.h"
#include "coordinate.h"
#include "transformation.h"

class DrawableObject
{
public:
  virtual ~DrawableObject();
  std::string getName() const;

  const std::list<Coordinate*>& getWorldCoordinates() const;
  const std::list<Coordinate*>& getWindowCoordinates() const;
  const std::list<Coordinate*>& getClippingCoordinates() const;

  static Coordinate getGeometricCenter(const std::list<Coordinate*>&);
  static void destroyList(std::list<Coordinate*>& coordinates);

  void apply(Transformation&);
  bool isDrawable() { return this->_isDrawable; }

  /**
   * Apply the `Transformation` to the `this->worldCoordinates` creating the Window Coordinates and
   * do the objects clipping accordingly to the `Axes` clipping limits on Window Coordinates.
   */
  virtual void updateWindowCoordinates(const Transformation&);
  virtual void updateClippingCoordinates(const Axes&);

  /**
   * Making operator<< virtual?
   * https://stackoverflow.com/questions/4571611/making-operator-virtual
   */
  friend std::ostream& operator<<(std::ostream &output, const DrawableObject &object);
  virtual void printMyself(std::ostream &output) const;

protected:
  DrawableObject(std::string name);
  DrawableObject(std::string name, std::list<Coordinate*> worldCoordinates);

  bool _isDrawable;
  std::string name;

  std::list<Coordinate*> worldCoordinates;
  std::list<Coordinate*> windowCoordinates;
  std::list<Coordinate*> clippingCoordinates;
};
#endif // GTKMM_APP_DRAWABLE_OBJECT
