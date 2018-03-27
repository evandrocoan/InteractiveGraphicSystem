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
#include <gtkmm/buttonbox.h>
#include <gtkmm/comboboxtext.h>
#include <gtkmm/grid.h>
#include <glibmm/refptr.h>

#include "viewport.h"
#include "debugger.h"
#include "viewwindow.h"
#include "addobject.h"
#include "addtransformation.h"
#include "ChooseFileWindow.h"
#include "RwObjectService.h"

#define DEFAULT_ZOOM_SCALE  1.5
#define DEFAULT_ROTATE_ANGLE "10"
#define DEFAULT_MOVE_LENGTH "10"

class MainWindow
{

public:
  MainWindow();
  virtual ~MainWindow();

  void updateDropdownList();
  Gtk::Window& getWindow();

protected:
  Gtk::ComboBoxText objects_list;
  Gtk::Window       window;

  ViewPort          viewPort;
  AddObject         addObject;
  AddTransformation addTransformation;

  Gtk::Button button_move_up;
  Gtk::Button button_move_down;
  Gtk::Button button_move_left;
  Gtk::Button button_move_right;

  Gtk::Button button_move_center;

  Gtk::Button button_rotate_left;
  Gtk::Button button_rotate_right;

  Gtk::Button button_zoom_in;
  Gtk::Button button_zoom_out;

  Gtk::Button button_add_object;
  Gtk::Button button_delete_object;

  Gtk::Button button_open_file;
  Gtk::Button button_save_file;


  Gtk::Box main_box;
  Gtk::Box left_box;

  Gtk::Frame left_frame;
  Gtk::Frame right_frame;

  Gtk::Grid grid_move;
  Gtk::Grid grid_zoom;
  Gtk::Grid grid_list_obj;
  Gtk::Grid grid_file;

  Gtk::Grid grid_rotate;
  Gtk::Grid grid_other;

  Gtk::Entry entry_move_length;
  Gtk::Entry entry_zoom_scale;
  Gtk::Entry entry_rotate_angle;

  void setupButtons();
  void connectButtons();

  void on_button_move_up();
  void on_button_move_down();
  void on_button_move_left();
  void on_button_move_right();

  void on_button_move_center();

  void on_button_rotate_left();
  void on_button_rotate_right();

  void on_button_zoom_in();
  void on_button_zoom_out();

  void on_button_add_object();
  void on_button_delete_object();

  RwObjectService rw_object_service;
  ChooseFileWindow* choose_file_window;
  std::list<DrawableObject*> get_drawable_objects();
  void on_button_open_file();
  void on_button_save_file();

};

#endif // GTKMM_APP_MAIN_WINDOW
