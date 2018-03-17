#include "addtransformation.h"

AddTransformation::AddTransformation(ViewPort* viewPort) :
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

  button_close.signal_clicked().connect( sigc::mem_fun(*this, &AddTransformation::on_button_close) );
  button_save_transformation.signal_clicked().connect( sigc::mem_fun(*this, &AddTransformation::on_button_save_transformation) );

  this->create_action_tabs();
  this->create_scrolling_items_list();

  this->window.set_title("Add Transformation");
  this->window.set_border_width(12);
  this->window.add(m_hbox);
  this->window.show_all_children();
}

void AddTransformation::create_action_tabs()
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
void AddTransformation::create_scrolling_items_list()
{
  //Fill the ListViewText:
  m_ListViewText.set_column_title(0, "Transformations");

  //Add the TreeView, inside a ScrolledWindow, with the button underneath
  m_ScrolledWindow.add(m_ListViewText);

  //Only show the scrollbars when they are necessary
  m_ScrolledWindow.set_policy(Gtk::PolicyType::POLICY_NEVER, Gtk::PolicyType::POLICY_AUTOMATIC);

  LOG(4, "Gtk::ScrolledWindow::property_max_content_width(): %d", m_ScrolledWindow.property_max_content_width());
  m_ScrolledWindow.set_min_content_width(200);

  m_hbox.pack_start(m_ScrolledWindow, true, true);
}

AddTransformation::~AddTransformation()
{
}

Gtk::Window* AddTransformation::getWindow()
{
  return &this->window;
}

void AddTransformation::on_button_save_transformation()
{
  int          current_page_index = m_notebook.get_current_page();
  Gtk::Widget* current_page_widget = m_notebook.get_nth_page(current_page_index);

  std::string main_value_value  = main_value_field.get_text().raw();
  std::string current_page_text = (std::string) m_notebook.get_tab_label_text(*current_page_widget);

  std::string name = tfm::format("%s %s", current_page_text, main_value_value);
  LOG(4, "%s", name);

  int x_coord = atoi(main_value_value.c_str());

  guint row_number = m_ListViewText.append(name);
  m_ListViewText.set_text(row_number, 0, name);

  if(current_page_text == "Translation")
  {
    this->transformation.add_translation(name, Coordinate(x_coord, 1, 1));
  }
  else if(current_page_text == "Rotation")
  {
  }
  else if(current_page_text == "Scaling")
  {
  }
  else
  {
    LOG(1, "");
    LOG(1, "");
    LOG(1, "ERROR! Current page used: %s", current_page_text);
  }
}

void AddTransformation::on_button_close()
{
  this->window.close();
  this->viewPort->apply(this->object_name, this->transformation);
}

