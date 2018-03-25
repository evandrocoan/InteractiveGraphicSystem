#ifndef GTKMM_APP_VIEWPORT
#define GTKMM_APP_VIEWPORT

#include <iostream>
#include <iomanip>

#include "traits.h"

#include "line.h"
#include "polygon.h"

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
class ViewPort
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
   * Lines representing the clipping window saved on `_clippingWindow` on viewport coordinates:
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
  Line* x1_axe;
  Line* x2_axe;
  Line* y1_axe;
  Line* y2_axe;

  Polygon _clippingWindow;

  std::list<Coordinate*> getCoordinates();

  void updateClippingWindowSize(int width, int height);

  friend std::ostream& operator<<(std::ostream &output, const ViewPort &object);

protected:
  /**
   * The coordinates use to represent the `_clippingWindow` X Y axis with `ViewPort` coordinates:
   *
   * _firstCoordinate       _forthCoordinate
   *          +-------------------+
   *          |                   |
   *          |                   |
   *          |                   |
   *          |                   |
   *          |                   |
   *          +-------------------+
   * _secondCoordinate      _thirdCoordinate
   *
   *
   * Now, the same coordinates represented on `ViewWindow` coordinates:
   *
   * _secondCoordinate       _thirdCoordinate
   *          +-------------------+
   *          |                   |
   *          |                   |
   *          |                   |
   *          |                   |
   *          |                   |
   *          +-------------------+
   * _firstCoordinate        _forthCoordinate
   */
  Coordinate* _firstCoordinate;
  Coordinate* _secondCoordinate;
  Coordinate* _thirdCoordinate;
  Coordinate* _forthCoordinate;
};

#endif
