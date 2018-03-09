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
  MainWindow*     mainWindow;
  DrawOptionsBox* drawOptionsBox;

  Gtk::Box      m_vbox;
  Gtk::Notebook m_notebook;

  string                 new_object_name;
  std::list<Coordinate*> polygon_cord_list;

  Gtk::Grid line_grid;
  Gtk::Grid point_grid;
  Gtk::Grid polygn_grid;

  Gtk::Button button_close;
  Gtk::Button button_save_line;
  Gtk::Button button_save_wire;
  Gtk::Button button_save_point;
  Gtk::Button button_add_coordenate;

  Gtk::Entry point_x_field;
  Gtk::Entry point_y_field;
  Gtk::Entry point_name_field;

  Gtk::Entry line_x1_field;
  Gtk::Entry line_y1_field;
  Gtk::Entry line_y2_field;
  Gtk::Entry line_x2_field;
  Gtk::Entry line_name_field;

  Gtk::Label info_label;
  Gtk::Label point_x_label;
  Gtk::Label point_y_label;

  Gtk::Entry wire_y_field;
  Gtk::Entry polygon_x_field;
  Gtk::Entry polygon_name_field;

  Gtk::Label line_x1_label;
  Gtk::Label line_y1_label;

  Gtk::Label line_x2_label;
  Gtk::Label line_y2_label;

  Gtk::Label polygon_x_label;
  Gtk::Label polygon_y_label;

  void on_button_close();
  void on_button_save_point();
  void on_button_save_line();
  void on_button_save_polygon();

  void on_button_add_coordinate();
  void _close_updating_list();
};
#endif //GTKMM_APP_ADD_OBJECT_WINDOW
