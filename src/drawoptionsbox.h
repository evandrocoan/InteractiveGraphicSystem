#ifndef GTKMM_APP_DRAW_OPTIONS_BOX
#define GTKMM_APP_DRAW_OPTIONS_BOX

/* Include any library as need to use other's components such as
 * gtkmm/button or gtkmm/frame.
 */
#include <gtkmm/frame.h>
#include <gtkmm/button.h>
#include <gtkmm/buttonbox.h>
#include <gtkmm/comboboxtext.h>
#include <gtkmm/grid.h>
#include "mainwindow.h"
#include "viewwindow.h"
#include "addobjectwindow.h"

#define DEFAULT_MOVE_LENGTH "10"
#define DEFAULT_ZOOM_SCALE 1.5

class DrawOptionsBox: public Gtk::Frame
{
public:
  DrawOptionsBox(const Glib::ustring& title, gint spacing, Gtk::ButtonBoxStyle layout, MainWindow* mainWindow);
  virtual ~DrawOptionsBox();
  void update_list_object();

protected:
  MainWindow*       mainWindow;
  AddObjectWindow*  add_object_window;

  Gtk::ComboBoxText objects_list;

  Gtk::Grid grid_move;
  Gtk::Grid grid_zoom;
  Gtk::Grid grid_list_obj;

  Gtk::Entry entry_move_length;
  Gtk::Entry entry_zoom_scale;

  Gtk::Button button_move_up;
  Gtk::Button button_move_down;
  Gtk::Button button_move_left;
  Gtk::Button button_move_right;

  Gtk::Button button_zoom_in;
  Gtk::Button button_zoom_out;

  Gtk::Button button_add_object;
  Gtk::Button button_delete_obj;

  void on_button_move_up();
  void on_button_move_down();
  void on_button_move_left();
  void on_button_move_right();

  void on_button_zoom_in();
  void on_button_zoom_out();

  void on_button_add_object();
  void on_button_delete_object();
};

#endif // GTKMM_APP_DRAW_OPTIONS_BOX

