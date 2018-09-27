#ifndef GTKMM_APP_CURVE
#define GTKMM_APP_CURVE

class Line;

#include "axes.h"
#include "coordinate.h"
#include "drawableobject.h"

enum CurveType
{
  BEZIER,
  BSPLINE
};


std::ostream& operator<<(std::ostream &output, const CurveType object);

class Curve : public DrawableObject
{
public:
  Curve(std::string name, std::list<Coordinate*>, Coordinate _borderColor, Coordinate _fillingColor, CurveType);
  ~Curve();

  bool visible_on_gui;
  std::list<Line*> lines;

  virtual void updateClippingCoordinates(const Axes&);

protected:
  CurveType curve_type;

  /**
   * http://www.skytopia.com/project/articles/compsci/clipping.html
   *
   * @return true if the line should be drawn, false if the line should be skipped drawing
   */
  bool _bezier(const Axes&);

  /**
   * https://en.wikipedia.org/wiki/Cohen%E2%80%93Sutherland_algorithm
   */
  bool _bspline(const Axes&);
};

#endif // GTKMM_APP_CURVE
