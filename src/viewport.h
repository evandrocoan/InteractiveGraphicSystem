#ifndef GTKMM_APP_VIEWPORT
#define GTKMM_APP_VIEWPORT

#include <iostream>
#include <iomanip>

#include "clippingwindow.h"

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

  ClippingWindow axes;

  ClippingWindow& getCoordinates();

  void updateClippingWindowSize(int width, int height);

  friend std::ostream& operator<<(std::ostream &output, const ViewPort &object);
};

#endif
