
#ifndef GTKMM_APP_VIEW_WINDOW
#define GTKMM_APP_VIEW_WINDOW

#include <ostream>
#include <iomanip>

#define MIN_WIDTH 15
#define MIN_HEIGHT 15
#define MAX_HEIGHT 50000000
#define MAX_WIDTH 50000000

class ViewWindow
{
public:
  ViewWindow();
  virtual ~ViewWindow();

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

  friend std::ostream& operator<<(std::ostream &output, const ViewWindow &object);
};

#endif
