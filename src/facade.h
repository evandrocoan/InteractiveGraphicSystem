#ifndef GTKMM_APP_FACADE
#define GTKMM_APP_FACADE

#include <string>
#include <vector>

#include "debugger.h"
#include "noncopyable.h"

#include "world.h"
#include "viewwindow.h"
#include "drawingarea.h"
#include "displayfile.h"

/**
 * A header only communication interface between the application logic and the user graphical
 * interface.
 */
class Facade : public NonCopyable
{
  friend class RwObjectService;

public:
  DrawingArea& drawingArea()       { return this->_drawingArea; }
  const DisplayFile<DrawableObject*>& displayFile() { return this->_world.displayFile(); }

  void addPoint(std::string name, int x, int y, int z, Coordinate _borderColor=_origin_coordinate_value)
  {
    this->_world.addPoint(name, x, y, z, _borderColor); }

  void addLine(std::string name, int x1, int y1, int z1, int x2, int y2, int z2,
      Coordinate _borderColor=_origin_coordinate_value,
      LineClippingType type=LineClippingType::LIANG_BARSKY)
  {
    this->_world.addLine(name, x1, y1, z1, x2, y2, z2, _borderColor, type); }

  void addPolygon(std::string name, std::vector<big_double> points,
      Coordinate _borderColor=_origin_coordinate_value,
      Coordinate _fillingColor=_origin_coordinate_value,
      CurveType type=CurveType::POLYGON)
  {
    this->_world.addPolygon(name, points, _borderColor, _fillingColor, type); }

  void addPolygon(std::string name, std::vector<Coordinate*> points,
      Coordinate _borderColor=_origin_coordinate_value,
      Coordinate _fillingColor=_origin_coordinate_value,
      CurveType type=CurveType::POLYGON)
  {
    this->_world.addPolygon(name, points, _borderColor, _fillingColor, type); }

  void addPolyhedron(std::string name, std::vector<Coordinate*> points,
              std::vector<int> _line_segments, int _facet_size,
              Coordinate _borderColor=_origin_coordinate_value,
              Coordinate _fillingColor=_origin_coordinate_value)
  {
    this->_world.addPolyhedron(name, points, _line_segments, _facet_size, _borderColor, _fillingColor); }

  void setLineClipping(LineClippingType type)
  {
    this->_world.setLineClipping(type); }

  void setProjection(Projection type, big_double projection) { this->_viewWindow.setProjection(type, projection); }
  void removeObject(std::string name) { this->_world.removeObject(name); }

  void move(Coordinate moves)        { this->_viewWindow.move(moves);        }
  void zoom(Coordinate factors)      { this->_viewWindow.zoom(factors);      }
  void rotate(Coordinate coordinate) { this->_viewWindow.rotate(coordinate); }

  void queue_draw() { this->_drawingArea.queue_draw(); }
  void apply(std::string object_name, Transformation& matrices) { this->_world.apply(object_name, matrices); }

  Facade() : _drawingArea(_world, _viewWindow)
  {
    using namespace std::placeholders;

    // ViewWindow observe DrawingArea size update
    this->_updateViewPortSize = this->_drawingArea.addObserver(std::bind(&ViewWindow::updateViewPortSize, &this->_viewWindow, _1, _2));

    // ViewWindow observe World object creation/deletion
    this->_updateObjectCoordinates = this->_world.addObserver(std::bind(&Facade::updateObjectCoordinates, this, _1));

    // World observe ViewWindow coordinates update
    this->_updateAllObjectCoordinates = this->_viewWindow.addObserver(std::bind(&Facade::updateAllObjectCoordinates, this, _1, _2));
  }

  /**
   * Updates an object Window coordinates.
   *
   * @param `object` we receive a nullptr when a object was removed and we just want to draw the
   *        screen without it.
   */
  void updateObjectCoordinates(DrawableObject* object)
  {
    if( object != nullptr ) {
      this->_world.updateObjectCoordinates(object, this->_viewWindow.transformation(), this->_viewWindow.axes());
    }
    this->_updateDropdownList();
    this->_drawingArea.queue_draw();
  }

  /**
   * Updates all objects Window Coordinates.
   */
  void updateAllObjectCoordinates(const Transformation& transformation, const Axes& axes)
  {
    this->_world.updateAllObjectCoordinates(transformation, axes);
    this->_drawingArea.queue_draw();
  }

  ~Facade() {
    bool result;

    result = this->_updateViewPortSize.disconnect();
    LOG(1, "Disconnecting the object `_updateViewPortSize` from its observer: %s", result);

    result = this->_updateObjectCoordinates.disconnect();
    LOG(1, "Disconnecting the object `_updateObjectCoordinates` from its observer: %s", result);

    result = this->_updateAllObjectCoordinates.disconnect();
    LOG(1, "Disconnecting the object `_updateAllObjectCoordinates` from its observer: %s", result);
  }

  /**
   * Implementations types for the Observer Design Pattern with C++ 11 templates and function
   * pointers, instead of tight coupled inheritance.
   */
  typedef Signal<> UpdateDropdownList;

  UpdateDropdownList::Connection addObserver(const UpdateDropdownList::Callback& callback) {
    return this->_updateDropdownList.connect(callback);
  }

protected:
  UpdateDropdownList _updateDropdownList;

  World       _world;
  ViewWindow  _viewWindow;
  DrawingArea _drawingArea;

  DrawingArea::UpdateViewPortSize::Connection        _updateViewPortSize;
  World::UpdateObjectCoordinates::Connection         _updateObjectCoordinates;
  ViewWindow::UpdateAllObjectCoordinates::Connection _updateAllObjectCoordinates;
};

#endif // GTKMM_APP_FACADE
