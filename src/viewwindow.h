
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

  Signal<>::Connection addObserver(const Signal<>::Callback&);

  void zoom_in (float scale = 1.5);
  void zoom_out(float scale = 1.5);

  void move_up   (int length = 10);
  void move_down (int length = 10);
  void move_left (int length = 10);
  void move_right(int length = 10);

  void rotate_left (GTKMM_APP_MATRICES_DATATYPE angle = 10.0);
  void rotate_right(GTKMM_APP_MATRICES_DATATYPE angle = 10.0);

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
  float xMin;
  float yMin;
  float xMax;
  float yMax;

  /**
   * Prints a beauty version of the viewWindow when called on `std::cout<< viewWindow << std::end;`
   */
  friend std::ostream& operator<<(std::ostream &output, const ViewWindow &object);

  Signal<> callObservers;

  // Deprecate these
  Coordinate& getCoordinate();
  void setCoordinate(Coordinate coordinate);

  void initPoints();
  void setPoints();
  void applyTransformation(Transformation& transformation);

private:
  Coordinate coordinate;

  Coordinate top_left;
  Coordinate top_right;
  Coordinate bottom_left;
  Coordinate bottom_right;
};

#endif
