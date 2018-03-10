#ifndef GTKMM_APP_MAIN_WINDOW
#define GTKMM_APP_MAIN_WINDOW

/* Include any library as need to use other's components such as
 * gtkmm/button or gtkmm/frame.
 */
#include <gtkmm/window.h>
#include <gtkmm/box.h>
#include <gtkmm/frame.h>
#include <gtkmm/viewport.h>
#include <gtkmm/adjustment.h>
#include <glibmm/refptr.h>
#include "debugger.h"
#include "viewport.h"
#include "viewwindow.h"


class MainWindow : public Gtk::Window
{

public:
  MainWindow();
  virtual ~MainWindow();
  Viewport* getViewport();

protected:
  Viewport viewport;

  Gtk::Box main_box;
  Gtk::Box left_box;

  Gtk::Frame left_frame;
  Gtk::Frame right_frame;
};

#endif // GTKMM_APP_MAIN_WINDOW
