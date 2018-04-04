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

  const std::list<Coordinate*>& worldCoordinates() const;
  const std::list<Coordinate*>& windowCoordinates() const;
  const std::list<Coordinate*>& clippingCoordinates() const;

  static Coordinate getGeometricCenter(const std::list<Coordinate*>&);
  static void destroyList(std::list<Coordinate*>& coordinates);

  void apply(Transformation&);
  bool isDrawable() const { return this->_isDrawable;   }

  const Coordinate& borderColor()  const { return this->_borderColor;  }
  const Coordinate& fillingColor() const { return this->_fillingColor; }

  /**
   * Apply the `Transformation` to the `this->_worldCoordinates` creating the Window Coordinates and
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
  DrawableObject(std::string name, std::list<Coordinate*> _worldCoordinates);
  DrawableObject(std::string name, std::list<Coordinate*> _worldCoordinates,
      Coordinate _borderColor, Coordinate _fillingColor=_default_coordinate_value_parameter);

  std::string name;
  bool _isDrawable;

  Coordinate _borderColor;
  Coordinate _fillingColor;

  std::list<Coordinate*> _worldCoordinates;
  std::list<Coordinate*> _windowCoordinates;
  std::list<Coordinate*> _clippingCoordinates;
};
#endif // GTKMM_APP_DRAWABLE_OBJECT
