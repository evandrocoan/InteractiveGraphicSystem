#include "addtransformationwindow.h"

AddTransformationWindow::AddTransformationWindow(ViewPort* viewPort) :
      m_vbox(Gtk::ORIENTATION_VERTICAL),
      m_hbox(Gtk::ORIENTATION_HORIZONTAL),
      viewPort(viewPort),
      m_ListViewText(1),
      button_save_transformation("Save Transformation"),
      button_close("Close")
{
  LOG(2, "Entering...");
  main_value_field.set_text("90");
  main_value_field.set_placeholder_text("Name");

  translation_grid.set_column_homogeneous(true);
  translation_grid.set_row_spacing(10);

  rotation_grid.set_column_homogeneous(true);
  rotation_grid.set_row_spacing(10);

  scaling_grid.set_column_homogeneous(true);
  scaling_grid.set_row_spacing(10);

  button_close.signal_clicked().connect( sigc::mem_fun(*this, &AddTransformationWindow::on_button_close) );
  button_save_transformation.signal_clicked().connect( sigc::mem_fun(*this, &AddTransformationWindow::on_button_save_transformation) );

  this->create_action_tabs();
  this->create_scrolling_items_list();

  this->window.set_title("Add Transformation");
  this->window.set_border_width(12);
  this->window.add(m_hbox);
  this->window.show_all_children();
}

void AddTransformationWindow::create_action_tabs()
{
  m_notebook.append_page(translation_grid, "Translation");
  m_notebook.append_page(rotation_grid, "Rotation");
  m_notebook.append_page(scaling_grid, "Scaling");
  m_notebook.set_border_width(0);

  m_vbox.pack_start(m_notebook);
  m_vbox.pack_start(main_value_field, Gtk::PACK_SHRINK);
  m_vbox.pack_start(button_save_transformation, Gtk::PACK_SHRINK);
  m_vbox.pack_start(button_close, Gtk::PACK_SHRINK);
  m_hbox.pack_start(m_vbox, true, true);
}

/**
 * how to add text box in gtkmm:gtk::Listbox using c++
 * https://stackoverflow.com/questions/34253297/how-to-add-text-box-in-gtkmmgtklistbox-using-c
 */
void AddTransformationWindow::create_scrolling_items_list()
{
  //Fill the ListViewText:
  m_ListViewText.set_column_title(0, "Transformations");

  //Add the TreeView, inside a ScrolledWindow, with the button underneath:
  m_ScrolledWindow.add(m_ListViewText);

  //Only show the scrollbars when they are necessary:
  m_ScrolledWindow.set_policy(Gtk::PolicyType::POLICY_NEVER, Gtk::PolicyType::POLICY_AUTOMATIC);

  LOG(4, "Gtk::ScrolledWindow::property_max_content_width(): %d", m_ScrolledWindow.property_max_content_width());
  m_ScrolledWindow.set_min_content_width(200);

  m_hbox.pack_start(m_ScrolledWindow, true, true);
}

AddTransformationWindow::~AddTransformationWindow()
{
}

Gtk::Window* AddTransformationWindow::getWindow()
{
  return &this->window;
}

void AddTransformationWindow::on_button_save_transformation()
{
  guint row_number = m_ListViewText.append();
  m_ListViewText.set_text(row_number, 0, "Madrid");
}

void AddTransformationWindow::on_button_close()
{
  this->window.close();
}

