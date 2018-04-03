#ifndef GTKMM_APP_POLYGON
#define GTKMM_APP_POLYGON

#include "drawableobject.h"
#include "coordinate.h"

class Polygon : public DrawableObject
{
public:
  Polygon(std::string name, std::list<Coordinate*> coordinates);
  ~Polygon();

  virtual void updateClippingCoordinates(const Axes&);

protected:
  /**
   * http://www.pracspedia.com/CG/sutherlandhodgman.html
   * https://en.wikipedia.org/wiki/Sutherland%E2%80%93Hodgman_algorithm
   * https://github.com/nogenem/UFSC_CG-2015-2/blob/master/Projeto_1/Etapa_4/include/Clipping.hpp
   *
   * @return true if the line should be drawn, false if the line should be skipped drawing
   */
  bool _sutherlandHodgmanPolygonClip(const Axes&);
  void _sutherlandHodgmanClipLeft(const Axes& axes, std::vector<Coordinate>& input, std::vector<Coordinate>& temporary);
  void _sutherlandHodgmanClipRight(const Axes& axes, std::vector<Coordinate>& temporary, std::vector<Coordinate>& clippingResult);
  void _sutherlandHodgmanClipTop(const Axes& axes, std::vector<Coordinate>& clippingResult, std::vector<Coordinate>& temporary);
  void _sutherlandHodgmanClipBottom(const Axes& axes, std::vector<Coordinate>& temporary, std::vector<Coordinate>& clippingResult);
};

#endif // GTKMM_APP_POLYGON
