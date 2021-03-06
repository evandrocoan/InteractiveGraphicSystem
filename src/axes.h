
#ifndef GTKMM_APP_AXES
#define GTKMM_APP_AXES

#include "coordinate.h"

struct Axe
{
  Coordinate& a;
  Coordinate& b;

  Axe(Coordinate& first, Coordinate& second) : a(first), b(second) { }

  /**
   * Prints a beauty version of the object when called on `std::cout<< object << std::end;`
   */
  friend std::ostream& operator<<(std::ostream &output, const Axe &object)
  {
    output << object.a << object.b;
    return output;
  }
};

struct Axes
{
  Axes(big_double xWiMin, big_double yWiMin, big_double xWiMax, big_double yWiMax) :
      xWiMin(xWiMin + CLIPPING_WINDOW_MARGIN_DISTANCE),
      yWiMin(yWiMin + CLIPPING_WINDOW_MARGIN_DISTANCE),
      xWiMax(xWiMax - CLIPPING_WINDOW_MARGIN_DISTANCE),
      yWiMax(yWiMax - CLIPPING_WINDOW_MARGIN_DISTANCE),
      _clippingWindowCoordinates
      {
        new Coordinate(xWiMin + CLIPPING_WINDOW_MARGIN_DISTANCE, yWiMin + CLIPPING_WINDOW_MARGIN_DISTANCE, 1),
        new Coordinate(xWiMin + CLIPPING_WINDOW_MARGIN_DISTANCE, yWiMax - CLIPPING_WINDOW_MARGIN_DISTANCE, 1),
        new Coordinate(xWiMax - CLIPPING_WINDOW_MARGIN_DISTANCE, yWiMax - CLIPPING_WINDOW_MARGIN_DISTANCE, 1),
        new Coordinate(xWiMax - CLIPPING_WINDOW_MARGIN_DISTANCE, yWiMin + CLIPPING_WINDOW_MARGIN_DISTANCE, 1)
      },
      x1(*this->_clippingWindowCoordinates[3], *this->_clippingWindowCoordinates[0]),
      y1(*this->_clippingWindowCoordinates[0], *this->_clippingWindowCoordinates[1]),
      x2(*this->_clippingWindowCoordinates[1], *this->_clippingWindowCoordinates[2]),
      y2(*this->_clippingWindowCoordinates[2], *this->_clippingWindowCoordinates[3])
  {
  }

  ~Axes()
  {
    unsigned int size = this->_clippingWindowCoordinates._data.size();

    for( unsigned int index = 0; index < size; index++ )
    {
      delete this->_clippingWindowCoordinates[index];
    }
  }

  /**
   * Return one of these coordinates points accordingly to the indexes 0, 1, 2 and 3.
   */
  const Coordinate& operator[](unsigned int line) const { return *(this->_clippingWindowCoordinates[line]); }

  /**
   * The Clock-Wise coordinates use to represent the X Y axes with `Window` Normalized coordinates:
   *
   *   Second Coordinate[1]       Third Coordinate[2]
   *                              (xWiMax, yWiMax)
   *               +-------------------+
   *               |                   |
   *               |                   |
   *               |                   |
   *               |                   |
   *               |                   |
   *               +-------------------+
   *   First Coordinate[0]        Forth Coordinate[3]
   *   (xWiMin, yWiMin)
   */
  const big_double xWiMin;
  const big_double yWiMin;
  const big_double xWiMax;
  const big_double yWiMax;
  const Array<4, Coordinate*> _clippingWindowCoordinates;

  /**
   * Lines representing the Clipping Area saved on `Window` Normalized coordinates:
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
  Axe x1;
  Axe y1;
  Axe x2;
  Axe y2;

  /**
   * Prints a beauty version of the object when called on `std::cout<< object << std::end;`
   */
  friend std::ostream& operator<<(std::ostream &output, const Axes &object)
  {
    output << "Axes[x1"
         << object.x1 << ", y1"
         << object.y1 << ", x2"
         << object.x2 << ", y2"
         << object.y2 << "]";
    return output;
  }
};

#endif // GTKMM_APP_AXES
