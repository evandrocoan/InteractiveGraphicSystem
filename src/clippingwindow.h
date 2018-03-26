#ifndef GTKMM_APP_AXES
#define GTKMM_APP_AXES

#include "coordinate.h"

class ClippingWindow : public Array<4, Coordinate*>
{
public:
  ClippingWindow()
  {
      this->_data[0] = new Coordinate(0, 0, 1);
      this->_data[1] = new Coordinate(0, 0, 1);
      this->_data[2] = new Coordinate(0, 0, 1);
      this->_data[3] = new Coordinate(0, 0, 1);

      this->x1 = {this->_data[1], this->_data[2]};
      this->y1 = {this->_data[0], this->_data[1]};
      this->x2 = {this->_data[3], this->_data[0]};
      this->y2 = {this->_data[2], this->_data[3]};
  }

  ~ClippingWindow()
  {
  }

  Coordinate* getPoint(unsigned int index)
  {
    return this->_data[index];
  }

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
  void updateClippingWindowSize(int width, int height)
  {
    (*this->_data[0])[0] = clipping_window_margin_distance;
    (*this->_data[0])[1] = clipping_window_margin_distance;

    (*this->_data[1])[0] = clipping_window_margin_distance;
    (*this->_data[1])[1] = height - clipping_window_margin_distance;

    (*this->_data[2])[0] = width - clipping_window_margin_distance;
    (*this->_data[2])[1] = height - clipping_window_margin_distance;

    (*this->_data[3])[0] = width - clipping_window_margin_distance;
    (*this->_data[3])[1] = clipping_window_margin_distance;
  }

  friend std::ostream& operator<<(std::ostream &output, const ClippingWindow &object)
  {
    output << "ClippingWindow" << "["
        << "x1" << *object.x1[0] << *object.x1[1] << " "
        << "y1" << *object.y1[0] << *object.y1[1] << " "
        << "x2" << *object.x2[0] << *object.x2[1] << " "
        << "y2" << *object.y2[0] << *object.y2[1] << "]";

    return output;
  }
};

#endif // GTKMM_APP_AXES
