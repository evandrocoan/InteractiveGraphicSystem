#ifndef GTKMM_APP_LINE
#define GTKMM_APP_LINE

#include "axes.h"
#include "coordinate.h"
#include "drawableobject.h"

enum LineClippingType
{
  LIANG_BARSKY,
  COHEN_SUTHELAND
};

enum CohenSuthelandRegionCode
{
  INSIDE=0,
  LEFT=1,
  RIGHT=2,
  BOTTOM=4,
  TOP=8
};

std::ostream& operator<<(std::ostream &output, const LineClippingType object);
std::ostream& operator<<(std::ostream &output, const CohenSuthelandRegionCode object);

class Line : public DrawableObject
{
public:
  Line(std::string name, Coordinate* line_cord1, Coordinate* line_cord2,
          Coordinate _borderColor, LineClippingType, bool _isVisibleOnGui);
  ~Line();

  void setLineClipping(LineClippingType);
  virtual void updateClippingCoordinates(const Axes&);

protected:
  LineClippingType line_clipping_type;

  /**
   * http://www.skytopia.com/project/articles/compsci/clipping.html
   *
   * @return true if the line should be drawn, false if the line should be skipped drawing
   */
  bool _liangBaskyLineClip(const Axes&);

  /**
   * https://en.wikipedia.org/wiki/Cohen%E2%80%93Sutherland_algorithm
   */
  bool _cohenSuthelandLineClip(const Axes&);
  unsigned int _getCoordianteRegionCode(const Axes&, const Coordinate&);
};

#endif // GTKMM_APP_LINE
