#ifndef GTKMM_APP_ADD_TRANSFORMATION_WINDOW
#define GTKMM_APP_ADD_TRANSFORMATION_WINDOW

#include <string>
#include <iostream>

#include <gtkmm/window.h>
#include <gtkmm/grid.h>
#include <gtkmm/button.h>
#include <gtkmm/entry.h>
#include <gtkmm/label.h>
#include <gtkmm/notebook.h>
#include <gtkmm/box.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/listviewtext.h>

#include "point.h"
#include "line.h"
#include "polygon.h"
#include "viewport.h"
#include "coordinate.h"

class AddTransformation
{
public:
  AddTransformation(ViewPort* viewPort);
  ~AddTransformation();

  Gtk::Window* getWindow();
  std::string object_name;

protected:
  ViewPort* viewPort;
  Transformation transformation;

  Gtk::Window window;
  Gtk::Entry main_value_field;

  Gtk::Box      m_vbox;
  Gtk::Box      m_hbox;
  Gtk::Notebook m_notebook;

  Gtk::ListViewText   m_ListViewText;
  Gtk::ScrolledWindow m_ScrolledWindow;

  Gtk::Grid rotation_grid;
  Gtk::Grid scaling_grid;
  Gtk::Grid translation_grid;

  Gtk::Button button_close;
  Gtk::Button button_save_transformation;

  void on_button_close();
  void on_button_save_transformation();

  void create_action_tabs();
  void create_scrolling_items_list();
};
#endif //GTKMM_APP_ADD_TRANSFORMATION_WINDOW
