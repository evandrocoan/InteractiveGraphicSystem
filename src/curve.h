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

protected:
  virtual ~Curve();

  void create_lines(const Transformation&, std::vector<Coordinate*>);
  std::vector<Line*> lines;

public:
  const std::vector<Line*> getLines() const;
  void updateClippingCoordinates(const Axes&);
};

class BezierCurve : public Curve
{
public:
  BezierCurve(std::string name, std::vector<Coordinate*>, Coordinate _borderColor, Coordinate _fillingColor);

  virtual void updateWindowCoordinates(const Transformation&);
};

class BsplineCurve : public Curve
{
public:
  BsplineCurve(std::string name, std::vector<Coordinate*>, Coordinate _borderColor, Coordinate _fillingColor);

  virtual void updateWindowCoordinates(const Transformation&);
};

#endif // GTKMM_APP_CURVE
