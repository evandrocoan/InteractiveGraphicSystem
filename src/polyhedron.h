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
      std::vector<int> _line_segments, int _facet_size,
      Coordinate _borderColor, Coordinate _fillingColor);

  virtual ~Polyhedron();

  const std::vector<Polygon*> getPolygons() const;
  void updateWindowCoordinates(const Transformation&);
  void updateClippingCoordinates(const Axes&);

  int facetSize() { return _facet_size; }
  const std::vector<int>& lineSegments() { return _line_segments; }

private:
  int _facet_size;

  std::vector<Polygon*> _polygons;
  std::vector<int> _line_segments;
};

#endif // GTKMM_APP_POLYHEDRON
