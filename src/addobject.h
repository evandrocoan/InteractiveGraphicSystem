#ifndef GTKMM_APP_ADD_OBJECT_WINDOW
#define GTKMM_APP_ADD_OBJECT_WINDOW

#include <string>
#include <vector>
#include <iostream>

#include <gtkmm/window.h>
#include <gtkmm/grid.h>
#include <gtkmm/button.h>
#include <gtkmm/entry.h>
#include <gtkmm/label.h>
#include <gtkmm/notebook.h>
#include <gtkmm/radiobutton.h>
#include <gtkmm/box.h>

#include "facade.h"

class AddObject
{
public:
  AddObject(Facade &facade);
  ~AddObject();
  Gtk::Window& getWindow();

private:
  Gtk::Window window;
  Facade &facade;

  Gtk::Box      m_vbox;
  Gtk::Notebook m_notebook;

  std::string new_object_name;
  std::vector<big_double> polygon_coord_list;

  Gtk::Grid color_grid;
  Gtk::Grid line_grid;
  Gtk::Grid point_grid;
  Gtk::Grid polygn_grid;

  Gtk::Button button_close;
  Gtk::Button button_save_line;
  Gtk::Button button_save_wire;
  Gtk::Button button_save_point;
  Gtk::Button button_add_coordenate;

  Gtk::Entry insert_border_color_field_r;
  Gtk::Entry insert_border_color_field_g;
  Gtk::Entry insert_border_color_field_b;
  Gtk::Entry insert_filling_color_field_r;
  Gtk::Entry insert_filling_color_field_g;
  Gtk::Entry insert_filling_color_field_b;

  Gtk::Entry point_name_field;
  Gtk::Entry point_x_field;
  Gtk::Entry point_y_field;

  Gtk::Entry line_name_field;
  Gtk::Entry line_x1_field;
  Gtk::Entry line_y1_field;
  Gtk::Entry line_y2_field;
  Gtk::Entry line_x2_field;

  Gtk::Entry polygon_name_field;
  Gtk::Entry wire_y_field;
  Gtk::Entry wire_x_field;

  Gtk::Label insert_border_color_label;
  Gtk::Label insert_filling_color_label;
  Gtk::Label insert_a_coordinate_label;
  Gtk::Label point_x_label;
  Gtk::Label point_y_label;

  Gtk::Label line_x1_label;
  Gtk::Label line_y1_label;
  Gtk::Label line_x2_label;
  Gtk::Label line_y2_label;

  Gtk::Label polygon_x_label;
  Gtk::Label polygon_y_label;

  Gtk::RadioButton liang_barsky_radiobutton;
  Gtk::RadioButton cohen_sutheland_radiobutton;

  LineClippingType line_clipping_type;

  void on_liang_radiobutton();
  void on_cohen_radiobutton();

  void on_button_close();
  void on_button_save_point();
  void on_button_save_line();
  void on_button_save_polygon();

  void on_button_add_coordinate();
  void _close_updating_list();
  std::string _get_field_name(Gtk::Entry&);

  Coordinate _get_rgb_color(Gtk::Entry& field_r, Gtk::Entry& field_g, Gtk::Entry& field_b);
};
#endif // GTKMM_APP_ADD_OBJECT_WINDOW
