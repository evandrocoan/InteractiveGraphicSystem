#ifndef GTKMM_APP_WORLD
#define GTKMM_APP_WORLD

#include "debugger.h"

#include "line.h"
#include "curve.h"
#include "point.h"
#include "polygon.h"
#include "polyhedron.h"

#include "displayfile.h"
#include "subject_controller.h"

class World
{
  friend class DrawingArea;
  friend class RwObjectService;

public:
  World();
  ~World();

  void addPoint(const std::string name, const int, const int, const int, Coordinate _borderColor);
  void addLine(const std::string name, const int, const int, const int, const int, const int, const int,
      Coordinate _borderColor, LineClippingType type=LineClippingType::LIANG_BARSKY, bool _isVisibleOnGui=true);

  void addPolygon(const std::string name, const std::vector<big_double>, Coordinate _borderColor, Coordinate _fillingColor, CurveType type);
  void addPolygon(const std::string name, const std::vector<Coordinate*>, Coordinate _borderColor, Coordinate _fillingColor, CurveType type);

  void addPolyhedron(std::string name, std::vector<Coordinate*> points, std::vector<int> _line_segments,
      std::vector<int> _facets_count, Coordinate _borderColor, Coordinate _fillingColor);

  void removeObject(const std::string name);
  void setLineClipping(LineClippingType);

  const DisplayFile<DrawableObject*>& displayFile() const { return this->_displayFile; }

  void draw_xy_axes();
  void apply(Transformation& matrices);
  void apply(const std::string object_name, Transformation& matrices);

  void updateAllObjectCoordinates(const Transformation&, const Axes&);
  static void updateObjectCoordinates(DrawableObject*, const Transformation&, const Axes&);

  /**
   * Implementations types for the Observer Design Pattern with C++ 11 templates and function
   * pointers, instead of tight coupled inheritance.
   */
  typedef Signal<DrawableObject*> UpdateObjectCoordinates;
  UpdateObjectCoordinates::Connection addObserver(const UpdateObjectCoordinates::Callback&);

protected:
  UpdateObjectCoordinates _updateObjectCoordinates;

  DisplayFile<Line*> _lines;
  DisplayFile<Curve*> _curves;
  DisplayFile<Point*> _points;
  DisplayFile<Polygon*> _polygons;

  DisplayFile<Polyhedron*> _polyhedrons;
  DisplayFile<DrawableObject*> _displayFile;
};

#endif // GTKMM_APP_WORLD
