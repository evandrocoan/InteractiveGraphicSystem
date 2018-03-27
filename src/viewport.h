#ifndef GTKMM_APP_VIEWPORT
#define GTKMM_APP_VIEWPORT

#include <iostream>
#include <iomanip>

#include "coordinate.h"

/**
 * The Drawing Area Widget
 * https://developer.gnome.org/gtkmm-tutorial/stable/chapter-drawingarea.html.en
 *
 * Drawing Straight Lines
 * https://developer.gnome.org/gtkmm-tutorial/stable/sec-cairo-drawing-lines.html.en#cairo-example-lines
 *
 * Viewport: a Porta de Visualização é a área do vídeo
 * onde será desenhado o que se deseja mostrar.
 * – Representado num sistema de coordenadas parcialmente
 * invertido, onde o eixo Y aponta para baixo.
 * – Tamanho fixo ou não controlado pela aplicação.
 *
 * The ViewPort is the computer screen which has the (0, 0) start point on the upper-left point,
 * instead of the down-left point as usual. And there, is where the ViewWindow has its starts point.
 *
 * And beyond these two systems, there is the world which has its start point symmetric with the
 * ViewWindow.
 */
class ViewPort : public Array<4, Coordinate*>
{
public:
  ViewPort();
  virtual ~ViewPort();

   /**
    * `ViewPort` coordinates:
    *
    *   (xMin, yMin) usually (0, 0)
    *         +-------------------+
    *         |                   |
    *         |                   |
    *         |                   |
    *         |                   |
    *         |                   |
    *         +-------------------+
    *                        (xMax, yMax) something like (600, 500)
    */
  int xMin;
  int yMin;
  int xMax;
  int yMax;

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
   *
   * `getPoint(int)` return one of these points accordingly to the indexes 0, 1, 2 and 3.
   */
  Coordinate* getPoint(unsigned int index);

  /**
   * Update the clipping window coordinates represented on the 4 internal points accessed by
   * `getPoint(int)`.
   *
   * @param width   the total view port widget width
   * @param height  the total view port widget height
   */
  void updateClippingWindowSize(int width, int height);

  /**
   * Prints a friend representation of the view port.
   */
  friend std::ostream& operator<<(std::ostream &output, const ViewPort &object);
};

#endif
