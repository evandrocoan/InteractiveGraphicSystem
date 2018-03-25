
#ifndef GTKMM_APP_VIEW_WINDOW
#define GTKMM_APP_VIEW_WINDOW

#include "coordinate.h"
#include "transformation.h"
#include "debugger.h"

#define MIN_WIDTH 15
#define MIN_HEIGHT 15
#define MAX_HEIGHT 50000000
#define MAX_WIDTH 50000000

class ViewWindow
{
public:
  ViewWindow(int Xwmin, int Ywmin, int Xwmax, int Ywmax);
  virtual ~ViewWindow();

  void zoom_in (float scale = 1.5);
  void zoom_out(float scale = 1.5);

  Coordinate getCoordinate();
  void setCoordinate(Coordinate coordinate);

  void move_up   (int length = 10);
  void move_down (int length = 10);
  void move_left (int length = 10);
  void move_right(int length = 10);


  void rotate_left (long double angle = 10.0);
  void rotate_right(long double angle = 10.0);
  void move_center();

  float xWmin;
  float yWmin;
  float xWmax;
  float yWmax;

  private: 

  Coordinate coordinate;
  Coordinate top_left;
  Coordinate top_right;
  Coordinate bottom_left;
  Coordinate bottom_right;

  void initPoints();
  void setPoints();

  
};

#endif
