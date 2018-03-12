
#ifndef GTKMM_APP_VIEW_WINDOW
#define GTKMM_APP_VIEW_WINDOW

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

  void move_up   (int length = 10);
  void move_down (int length = 10);
  void move_left (int length = 10);
  void move_right(int length = 10);

  float xWmin;
  float yWmin;
  float xWmax;
  float yWmax;
};

#endif
