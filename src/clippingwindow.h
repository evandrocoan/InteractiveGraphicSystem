#ifndef GTKMM_APP_AXES
#define GTKMM_APP_AXES

#include "coordinate.h"

class ClippingWindow : public Array<4, Coordinate*>
{
public:
  ClippingWindow();
  ~ClippingWindow();

  /**
   * Lines representing the clipping window saved on viewport coordinates:
   *
   *                 x1_axe
   *         +-------------------+
   *         |                   |
   *         |                   |
   * y1_axe  |                   | y2_axe
   *         |                   |
   *         |                   |
   *         +-------------------+
   *                 x2_axe
   *
   *
   * Now, the same lines represented on `ViewWindow` coordinates:
   *
   *                 x2_axe
   *         +-------------------+
   *         |                   |
   *         |                   |
   * y1_axe  |                   | y2_axe
   *         |                   |
   *         |                   |
   *         +-------------------+
   *                 x1_axe
   */
  Array<2, Coordinate*> x1;
  Array<2, Coordinate*> x2;
  Array<2, Coordinate*> y1;
  Array<2, Coordinate*> y2;

  /**
   * The coordinates use to represent the X Y axis with `ViewPort` coordinates:
   *
   * firstCoordinate       forthCoordinate
   *          +-------------------+
   *          |                   |
   *          |                   |
   *          |                   |
   *          |                   |
   *          |                   |
   *          +-------------------+
   * secondCoordinate      thirdCoordinate
   *
   *
   * Now, the same coordinates represented on `ViewWindow` coordinates:
   *
   * secondCoordinate       thirdCoordinate
   *          +-------------------+
   *          |                   |
   *          |                   |
   *          |                   |
   *          |                   |
   *          |                   |
   *          +-------------------+
   * firstCoordinate        forthCoordinate
   */
  void updateClippingWindowSize(int width, int height);

  Coordinate* getPoint(unsigned int index);

  friend std::ostream& operator<<(std::ostream &output, const ClippingWindow &object);
};

#endif // GTKMM_APP_AXES
