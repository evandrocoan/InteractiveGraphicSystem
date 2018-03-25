
#ifndef GTKMM_APP_VIEW_WINDOW
#define GTKMM_APP_VIEW_WINDOW

#include <ostream>
#include <iomanip>

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

  /**
   * Add an observer to this ViewWindow, which is notified every time the ViewWindow has some change
   * on any of its attributes.
   *
   * After register the observer callback and perform the first notification call on the just added
   * observer.
   *
   * @param `callback` a function pointer to the observer to be notified
   * @return           a Connection method allowing the observer to unsubscribe from observing this
   *                   object. Useful to avoid null pointer exceptions when the observer function
   *                   pointer is deleted.
   */
  Signal<>::Connection addObserver(const Signal<>::Callback&);

  void zoom_in (float scale = 1.5);
  void zoom_out(float scale = 1.5);

  void move_up   (int length = 10);
  void move_down (int length = 10);
  void move_left (int length = 10);
  void move_right(int length = 10);

  float xMin;
  float yMin;
  float xMax;
  float yMax;

  /**
   * Prints a beauty version of the viewWindow when called on `std::cout<< viewWindow << std::end;`
   */
  friend std::ostream& operator<<(std::ostream &output, const ViewWindow &object);

protected:
  Signal<> observerController;
};

#endif
