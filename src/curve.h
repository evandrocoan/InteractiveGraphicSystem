#ifndef GTKMM_APP_CURVE
#define GTKMM_APP_CURVE

class Line;

#include "axes.h"
#include "coordinate.h"
#include "drawableobject.h"

std::ostream& operator<<(std::ostream &output, const CurveType object);

class Curve : public DrawableObject
{
  using DrawableObject::DrawableObject;

public:
  std::vector<Line*> lines;
};

class BezierCurve : public Curve
{
public:
  BezierCurve(std::string name, std::vector<Coordinate*>, Coordinate _borderColor, Coordinate _fillingColor);
  ~BezierCurve();

  virtual void updateWindowCoordinates(const Transformation&);
  virtual void updateClippingCoordinates(const Axes&);
};

#endif // GTKMM_APP_CURVE
