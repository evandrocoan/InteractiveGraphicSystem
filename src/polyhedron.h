#ifndef GTKMM_APP_POLYHEDRON
#define GTKMM_APP_POLYHEDRON

class Polygon;

#include "axes.h"
#include "coordinate.h"
#include "drawableobject.h"

// https://en.wikipedia.org/wiki/Polyhedron
class Polyhedron : public DrawableObject
{
public:
  Polyhedron(std::string name, std::vector<Coordinate*>,
      std::vector<int> _line_segments, std::vector<int> _facets_count,
      Coordinate _borderColor, Coordinate _fillingColor);

  virtual ~Polyhedron();

  const std::vector<Polygon*> getPolygons() const;
  void updateWindowCoordinates(const Transformation&);
  void updateClippingCoordinates(const Axes&);

  const std::vector<int>& facetsCount() const { return _facets_count; }
  const std::vector<int>& lineSegments() const { return _line_segments; }

private:
  std::vector<Polygon*> _polygons;
  std::vector<int> _line_segments;
  std::vector<int> _facets_count;
};

#endif // GTKMM_APP_POLYHEDRON
