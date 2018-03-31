
#ifndef GTKMM_APP_VIEW_WINDOW
#define GTKMM_APP_VIEW_WINDOW

#include <ostream>
#include <iomanip>

#include "coordinate.h"
#include "transformation.h"
#include "debugger.h"

#include "subject_controller.h"

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
 */
class ViewWindow
{
public:
  ViewWindow();
  virtual ~ViewWindow();

  void zoom(long int step = 50);
  void move(long int horizontal_step = 50, long int vertical_step = 50);
  void rotate(Coordinate coordinate);
  void apply(Coordinate&);

  long double width();
  long double height();

  /**
   * `ViewWindow` coordinates:
   *
   *                        (xMax, yMax) something like (600, 500)
   *         +-------------------+
   *         |                   |
   *         |                   |
   *         |                   |
   *         |                   |
   *         |                   |
   *         +-------------------+
   *   (xMin, yMin) usually (0, 0)
   */
  constexpr static const long double xMin = -1.0;
  constexpr static const long double yMin = -1.0;
  constexpr static const long double xMax =  1.0;
  constexpr static const long double yMax =  1.0;

  /**
   * Prints a beauty version of the viewWindow when called on `std::cout<< viewWindow << std::end;`
   */
  friend std::ostream& operator<<(std::ostream &output, const ViewWindow &object);

  void callObservers();
  Signal<>::Connection addObserver(const Signal<>::Callback&);

protected:
  long int _width;
  long int _height;

  Coordinate _angle;
  Coordinate _windowCenter;

  Signal<> _observers;
  Transformation _transformation;
};

#endif // GTKMM_APP_VIEW_WINDOW
