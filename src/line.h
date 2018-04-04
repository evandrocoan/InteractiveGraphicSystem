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

std::ostream& operator<<(std::ostream &output, const LineClippingType object);

class Line : public DrawableObject
{
public:
  Line(std::string name, Coordinate* line_cord1, Coordinate* line_cord2, Coordinate _borderColor, LineClippingType);
  ~Line();

  virtual void updateClippingCoordinates(const Axes&);

protected:
  LineClippingType line_clipping_type;

  /**
   * http://www.skytopia.com/project/articles/compsci/clipping.html
   *
   * @return true if the line should be drawn, false if the line should be skipped drawing
   */
  bool _liangBaskyLineClip(const Axes&);
};

#endif // GTKMM_APP_LINE
