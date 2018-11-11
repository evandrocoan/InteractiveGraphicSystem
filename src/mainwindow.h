#ifndef GTKMM_APP_MAIN_WINDOW
#define GTKMM_APP_MAIN_WINDOW

/* Include any library as need to use other's components such as
 * gtkmm/button or gtkmm/frame.
 */
#include <gtkmm/window.h>
#include <gtkmm/box.h>
#include <gtkmm/frame.h>
#include <gtkmm/adjustment.h>
#include <gtkmm/buttonbox.h>
#include <gtkmm/comboboxtext.h>
#include <gtkmm/grid.h>
#include <glibmm/refptr.h>

#include "facade.h"
#include "addobject.h"
#include "error_message_dialog.h"

#include "choose_file_window.h"
#include "rw_object_service.h"

class MainWindow
{

public:
  MainWindow();
  virtual ~MainWindow();

  void updateDropdownList();
  Gtk::Window& getWindow();

protected:
  Gtk::Window       window;
  Gtk::ComboBoxText objects_list;

  Facade            facade;
  AddObject         addObject;

  RwObjectService   rw_object_service;
  ChooseFileWindow* choose_file_window;

  Gtk::Button button_move_up;
  Gtk::Button button_move_down;
  Gtk::Button button_move_left;
  Gtk::Button button_move_right;

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
  Gtk::Grid grid_rotate;

  Gtk::Entry entry_move_length;
  Gtk::Entry entry_zoom_scale;
  Gtk::Entry entry_rotate_angle;

  Gtk::RadioButton liang_barsky_radiobutton;
  Gtk::RadioButton cohen_sutheland_radiobutton;

  void on_liang_radiobutton();
  void on_cohen_radiobutton();

  void setupButtons();
  void connectButtons();
  void setDefaultTooltips();

  void on_button_move_up();
  void on_button_move_down();
  void on_button_move_left();
  void on_button_move_right();

  void on_button_rotate_left();
  void on_button_rotate_right();

  void on_button_zoom_in();
  void on_button_zoom_out();

  void on_button_add_object();
  void on_button_delete_object();
  void on_objects_list_change();

  void on_button_open_file();
  void on_button_save_file();

  // Transformation
  Transformation transformation;
  std::string object_name;

  TransformationType  transformation_type;
  TransformationPoint transformation_point;

  Gtk::Entry x_rotation_field;
  Gtk::Entry y_rotation_field;
  Gtk::Entry z_rotation_field;

  Gtk::Entry main_value_field_a;
  Gtk::Entry main_value_field_b;
  Gtk::Entry main_value_field_c;

  Gtk::Box            main_vertical_box;
  Gtk::ListViewText   list_view_text;
  Gtk::ScrolledWindow scrolled_window;

  Gtk::Grid coodinate_input_grid;

  Gtk::RadioButton translation_radiobutton;
  Gtk::RadioButton scaling_radiobutton;
  Gtk::RadioButton rotation_radiobutton;

  Gtk::RadioButton world_center_radiobutton;
  Gtk::RadioButton geometric_center_radiobutton;
  Gtk::RadioButton any_point_radiobutton;

  Gtk::Button button_apply;
  Gtk::Button button_save_transformation;
  Gtk::Button button_remove_transformation;

  void on_button_apply();
  void on_button_save_transformation();
  void on_button_remove_transformation();

  void on_translation_radiobutton();
  void on_scaling_radiobutton();
  void on_rotation_radiobutton();

  void on_world_center_rotation_radiobutton();
  void on_own_center_rotation_radiobutton();
  void on_given_coordinate_rotation_radiobutton();

  void create_action_tabs();
  void create_scrolling_items_list();
  void set_default_values_and_tooltips();

  void _addThingsToTheWindow();
  void _connectButtons();
  void _update_transmations_list();

private:
  void add_test_objects();
};

#endif // GTKMM_APP_MAIN_WINDOW
