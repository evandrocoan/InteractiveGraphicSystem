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
#include "error_message_dialog.h"

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

  // https://stackoverflow.com/questions/12218970/gtk-label-wrapping-in-a-dialog
  Gtk::Label entered_points_field;
  std::string new_object_name;

  std::vector<big_double> polygon_coord_list;
  std::vector<std::string> entered_points_text;

  Gtk::Grid color_grid;
  Gtk::Grid polygon_grid;

  Gtk::Button button_close;
  Gtk::Button button_save_line;
  Gtk::Button button_save_polygon;
  Gtk::Button button_save_bezier;
  Gtk::Button button_save_bspline;
  Gtk::Button button_save_point;
  Gtk::Button button_add_coordenate;
  Gtk::Button button_remove_coordenate;

  Gtk::Entry insert_border_color_field_r;
  Gtk::Entry insert_border_color_field_g;
  Gtk::Entry insert_border_color_field_b;
  Gtk::Entry insert_filling_color_field_r;
  Gtk::Entry insert_filling_color_field_g;
  Gtk::Entry insert_filling_color_field_b;

  Gtk::Entry polygon_name_field;
  Gtk::Entry wire_y_field;
  Gtk::Entry wire_x_field;

  Gtk::Label insert_border_color_label;
  Gtk::Label insert_filling_color_label;
  Gtk::Label insert_a_coordinate_label;

  Gtk::Label polygon_x_label;
  Gtk::Label polygon_y_label;

  void on_button_close();
  void on_button_save_point();
  void on_button_save_line();
  void on_button_save_polygon();
  void on_button_save_bezier();
  void on_button_save_bspline();

  void on_button_add_coordinate();
  void on_button_remove_coordinate();

  void _close_updating_list();

  std::string _get_field_name(Gtk::Entry&);
  Coordinate _get_rgb_color(Gtk::Entry& field_r, Gtk::Entry& field_g, Gtk::Entry& field_b);
};
#endif // GTKMM_APP_ADD_OBJECT_WINDOW
