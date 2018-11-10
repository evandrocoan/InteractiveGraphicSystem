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

class Line;

enum CurveType
{
  NOCURVE,
  POLYGON,
  BEZIER,
  BSPLINE
};

class DrawableObject
{
public:
  virtual ~DrawableObject();
  std::string getName() const;

  const std::vector<Coordinate*>& worldCoordinates() const;
  const std::vector<Coordinate*>& windowCoordinates() const;
  const std::vector<Coordinate*>& clippingCoordinates() const;

  static Coordinate getGeometricCenter(const std::vector<Coordinate*>&);

  /**
   * https://stackoverflow.com/questions/307082/cleaning-up-an-stl-list-vector-of-pointers
   * Cleaning up an STL list/vector of pointers
   */
  template<typename VectorType>
  static void destroyList(std::vector<VectorType>& elements)
  {
    while(!elements.empty())
    {
      // delete elements.front();
      elements.pop_back();
    }
  }

  void apply(Transformation&);
  bool isDrawable() const { return this->_isDrawable;   }
  bool isVisibleOnGui() const { return this->_isVisibleOnGui;   }

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
  DrawableObject(std::string name, std::vector<Coordinate*> _worldCoordinates);
  DrawableObject(std::string name, std::vector<Coordinate*> _worldCoordinates,
      Coordinate _borderColor,
      Coordinate _fillingColor=_default_coordinate_value_parameter,
      bool _isVisibleOnGui=true );

  /// The name of this object used to display it on the Graphics User Interface
  std::string name;

  /// Whether the object is inside the Window or out-of-sight
  bool _isDrawable;

  /// Whether the object should be displayed on the User Graphics Interface
  bool _isVisibleOnGui;

  Coordinate _borderColor;
  Coordinate _fillingColor;

  std::vector<Coordinate*> _worldCoordinates;
  std::vector<Coordinate*> _windowCoordinates;
  std::vector<Coordinate*> _clippingCoordinates;
};
#endif // GTKMM_APP_DRAWABLE_OBJECT
