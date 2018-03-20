#ifndef GTKMM_APP_ADD_TRANSFORMATION_WINDOW
#define GTKMM_APP_ADD_TRANSFORMATION_WINDOW

#include <string>
#include <iostream>

#include <gtkmm/grid.h>
#include <gtkmm/button.h>
#include <gtkmm/entry.h>
#include <gtkmm/label.h>
#include <gtkmm/notebook.h>
#include <gtkmm/box.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/listviewtext.h>
#include <gtkmm/radiobutton.h>

#include "point.h"
#include "line.h"
#include "polygon.h"
#include "viewport.h"
#include "coordinate.h"

class AddTransformation
{
public:
  AddTransformation(ViewPort &viewPort);
  ~AddTransformation();

  Gtk::Box& getBox();
  std::string object_name;
  RotationType rotation_type;

protected:
  ViewPort &viewPort;
  Transformation transformation;

  Gtk::Entry x_rotation_field;
  Gtk::Entry main_value_field_a;
  Gtk::Entry main_value_field_b;
  Gtk::Entry main_value_field_c;

  Gtk::RadioButton m_rb1;
  Gtk::RadioButton m_rb2;
  Gtk::RadioButton m_rb3;

  Gtk::Box      m_vbox;
  Gtk::Notebook m_notebook;

  Gtk::ListViewText   m_ListViewText;
  Gtk::ScrolledWindow m_ScrolledWindow;

  Gtk::Grid rotation_grid;
  Gtk::Grid scaling_grid;
  Gtk::Grid translation_grid;
  Gtk::Grid coodinate_input_grid;

  Gtk::Button button_apply;
  Gtk::Button button_save_transformation;
  Gtk::Button button_remove_transformation;

  void on_button_apply();
  void on_button_save_transformation();
  void on_button_remove_transformation();

  void on_world_rotation_button();
  void on_own_center_rotation_button();
  void on_given_coordinate_rotation_button();

  void create_action_tabs();
  void create_scrolling_items_list();

  void _update_transmations_list();
};
#endif //GTKMM_APP_ADD_TRANSFORMATION_WINDOW
