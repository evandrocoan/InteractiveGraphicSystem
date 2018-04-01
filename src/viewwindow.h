
#ifndef GTKMM_APP_VIEW_WINDOW
#define GTKMM_APP_VIEW_WINDOW

#include <ostream>
#include <iomanip>

#include "debugger.h"
#include "utilities.h"
#include "subject_controller.h"

#include "coordinate.h"
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
 */
class ViewWindow
{
public:
  ViewWindow();
  virtual ~ViewWindow();

  void zoom(Coordinate steps);
  void move(Coordinate steps);
  void rotate(Coordinate steps);

  const Transformation& transformation() const;

  big_double width();
  big_double height();

  /**
   * Prints a beauty version of the viewWindow when called on `std::cout<< viewWindow << std::end;`
   */
  friend std::ostream& operator<<(std::ostream &output, const ViewWindow &object);

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
  constexpr static const big_double xMin = -1.0;
  constexpr static const big_double yMin = -1.0;
  constexpr static const big_double xMax =  1.0;
  constexpr static const big_double yMax =  1.0;

  /**
   * Implementations types for the Observer Design Pattern with C++ 11 templates and function
   * pointers, instead of tight coupled inheritance.
   */
  typedef Signal<Transformation> ChangedSignal;
  typedef ChangedSignal::Callback ChangedCallback;
  typedef ChangedSignal::Connection ChangedConnection;

  ChangedConnection addObserver(const ChangedCallback&);

protected:
  Coordinate _angles;
  Coordinate _dimentions;
  Coordinate _windowCenter;

  void callObservers();
  ChangedSignal _observers;

  Transformation _transformation;
  Transformation _angle_rotation;
};

#endif // GTKMM_APP_VIEW_WINDOW
