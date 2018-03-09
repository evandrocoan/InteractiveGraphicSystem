#ifndef GTKMM_APP_ADD_OBJECT_WINDOW
#define GTKMM_APP_ADD_OBJECT_WINDOW

/* Include any library as need to use other's components such as
 * gtkmm/button or gtkmm/frame.
 */
#include <gtkmm/window.h>
#include <gtkmm/grid.h>
#include <gtkmm/button.h>
#include <gtkmm/entry.h>
#include <gtkmm/label.h>
#include <gtkmm/notebook.h>
#include <gtkmm/box.h>
#include <string>
#include <iostream>
#include "point.h"
#include "line.h"
#include "polygon.h"
#include "mainwindow.h"
#include "coordinate.h"

class DrawOptionsBox;

class AddObjectWindow : public Gtk::Window
{
public:
  AddObjectWindow(MainWindow* mainWindow, DrawOptionsBox* drawOptionsBox);
  ~AddObjectWindow();

protected:
  // Add here any components or frames
  MainWindow* mainWindow;
  DrawOptionsBox* drawOptionsBox;

  std::list<Coordinate*> polygon_cord_list;
  Gtk::Grid point_grid, line_grid, polygn_grid;

  Gtk::Notebook m_notebook;
  Gtk::Box m_vbox;

  Gtk::Button button_add_coordenate, button_save_line, button_close, button_save_wire, button_save_point;

  Gtk::Entry point_name_field, point_x_field, point_y_field;
  Gtk::Entry line_name_field, line_x1_field, line_x2_field, line_y1_field, line_y2_field;

  Gtk::Entry polygon_name_field, polygon_x_field, wire_y_field;
  Gtk::Label point_x_label, point_y_label;

  Gtk::Label line_x1_label, line_x2_label, line_y1_label, line_y2_label;
  Gtk::Label polygon_x_label, polygon_y_label, info_label;

  string new_object_name;

  void on_button_add_coordinate();
  void on_button_close();
  void on_button_save_point();
  void on_button_save_line();
  void on_button_save_polygon();
  void _close_updating_list();
};
#endif //GTKMM_APP_ADD_OBJECT_WINDOW
