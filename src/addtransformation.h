#ifndef GTKMM_APP_ADD_TRANSFORMATION_WINDOW
#define GTKMM_APP_ADD_TRANSFORMATION_WINDOW

#include <string>
#include <iostream>

#include <gtkmm/grid.h>
#include <gtkmm/button.h>
#include <gtkmm/entry.h>
#include <gtkmm/label.h>
#include <gtkmm/box.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/listviewtext.h>
#include <gtkmm/radiobutton.h>

#include "facade.h"
#include "error_message_dialog.h"

class AddTransformation
{
public:
  AddTransformation(Facade &facade);
  ~AddTransformation();

  Gtk::Box&   getBox();
  std::string object_name;

  TransformationType  transformation_type;
  TransformationPoint transformation_point;

protected:
  Facade& facade;
  Transformation transformation;

  Gtk::Entry x_rotation_field;
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
};
#endif // GTKMM_APP_ADD_TRANSFORMATION_WINDOW
