#ifndef GTKMM_APP_CURVE
#define GTKMM_APP_CURVE

class Line;

#include "axes.h"
#include "coordinate.h"
#include "drawableobject.h"

std::ostream& operator<<(std::ostream &output, const CurveType object);

class Curve : public DrawableObject
{
public:
  Curve(std::string name, std::vector<Coordinate*>, Coordinate _borderColor, Coordinate _fillingColor, CurveType);
  ~Curve();

  const bool visible_on_gui;
  virtual void updateWindowCoordinates(const Transformation&);
  virtual void updateClippingCoordinates(const Axes&);

protected:
  bool _bezier(const Axes&);
  bool _bezier(const Transformation&);

  bool _bspline(const Axes&);
  bool _bspline(const Transformation&);
};

#endif // GTKMM_APP_CURVE
