
#ifndef GTKMM_APP_VIEW_WINDOW
#define GTKMM_APP_VIEW_WINDOW

#include <ostream>
#include <iomanip>

#include "debugger.h"
#include "utilities.h"
#include "subject_controller.h"

#include "axes.h"
#include "noncopyable.h"
#include "transformation.h"

#define MIN_WIDTH 15
#define MIN_HEIGHT 15
#define MAX_HEIGHT 50000000
#define MAX_WIDTH 50000000

/**
 * Window: Retângulo que representa um recorte do
 * mundo 2D representado pelo Display File que será
 * mostrado no vídeo.
 * – Representado no sistema de coordenadas cartesiano.
 * – Tamanho variável (Zoom).
 * – Posição variável (navegação)
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
class ViewWindow : public NonCopyable
{
public:
  ViewWindow();
  virtual ~ViewWindow();

  void zoom(Coordinate steps);
  void move(Coordinate steps);
  void rotate(Coordinate steps);

  const Axes& axes() const                     { return this->_axes; };
  const Transformation& transformation() const { return this->_transformation; }

  const Coordinate& point(unsigned int index) const { return *(this->_axes._clippingWindowCoordinates[index]); }
  const Coordinate  viewPort(unsigned int index) const {
    return this->convertCoordinateToViewPort( *(this->_axes._clippingWindowCoordinates[index]) );
  }

  Coordinate convertCoordinateToViewPort(const Coordinate&) const;
  void updateViewPortSize(big_double width, big_double height);

  big_double width()  { return this->_dimentions[0]; }
  big_double height() { return this->_dimentions[1]; }

  /**
   * The Drawing Area Widget `ViewPort` Real Screen Coordinates:
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
  int xVpMin;
  int yVpMin;
  int xVpMax;
  int yVpMax;

  /**
   * Fixed Normalized `Window` Coordinates:
   *
   *                        (xMax, yMax) something like (1, 1)
   *         +-------------------+
   *         |                   |
   *         |                   |
   *         |                   |
   *         |                   |
   *         |                   |
   *         +-------------------+
   *   (xMin, yMin) usually (-1, -1)
   */
  constexpr static const big_double xWiMin = -1.0;
  constexpr static const big_double yWiMin = -1.0;
  constexpr static const big_double xWiMax =  1.0;
  constexpr static const big_double yWiMax =  1.0;

  /**
   * Prints a beauty version of the viewWindow when called on `std::cout<< viewWindow << std::end;`
   */
  friend std::ostream& operator<<(std::ostream &output, const ViewWindow &object);

  /**
   * Implementations types for the Observer Design Pattern with C++ 11 templates and function
   * pointers, instead of tight coupled inheritance.
   */
  typedef Signal<const Transformation&, const Axes&> UpdateAllObjectCoordinates;
  UpdateAllObjectCoordinates::Connection addObserver(const UpdateAllObjectCoordinates::Callback&);

protected:
  Axes _axes;
  UpdateAllObjectCoordinates _updateAllObjectCoordinates;

  Coordinate _angles;
  Coordinate _dimentions;
  Coordinate _windowCenter;

  Transformation _transformation;
  Transformation _angle_rotation;

  void callObservers();
};

#endif // GTKMM_APP_VIEW_WINDOW
