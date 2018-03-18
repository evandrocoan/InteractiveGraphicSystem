#include "addtransformation.h"

AddTransformation::AddTransformation(ViewPort &viewPort) :
      m_vbox(Gtk::ORIENTATION_VERTICAL),
      m_hbox(Gtk::ORIENTATION_HORIZONTAL),
      viewPort(viewPort),
      m_ListViewText(1),
      rotation_type(RotationType::ON_ITS_OWN_CENTER),
      m_rb1("Rotate around world center"),
      m_rb2("Rotate around the object own geometric center"),
      m_rb3("Rotate around an arbitrary point"),
      button_save_transformation("Save Transformation"),
      button_remove_transformation("Remove Transformation"),
      button_close("Close")
{
  LOG(2, "Entering...");
  m_rb1.set_active();
  m_rb2.join_group(m_rb1);
  m_rb3.join_group(m_rb1);

  x_rotation_field.set_text("90");
  x_rotation_field.set_placeholder_text("Name");
  main_value_field_a.set_text("100");
  main_value_field_a.set_placeholder_text("Name");
  main_value_field_b.set_text("0");
  main_value_field_b.set_placeholder_text("Name");
  main_value_field_c.set_text("0");
  main_value_field_c.set_placeholder_text("Name");

  translation_grid.set_column_homogeneous(true);
  translation_grid.set_row_spacing(10);

  rotation_grid.set_column_homogeneous(true);
  rotation_grid.set_row_spacing(10);
  rotation_grid.attach(x_rotation_field, 1, 1, 1, 1);
  rotation_grid.attach(m_rb1, 1, 2, 1, 1);
  rotation_grid.attach(m_rb2, 1, 3, 1, 1);
  rotation_grid.attach(m_rb3, 1, 4, 1, 1);

  scaling_grid.set_column_homogeneous(true);
  scaling_grid.set_row_spacing(10);

  button_close.signal_clicked().connect( sigc::mem_fun(*this, &AddTransformation::on_button_close) );
  button_save_transformation.signal_clicked().connect( sigc::mem_fun(*this, &AddTransformation::on_button_save_transformation) );
  button_remove_transformation.signal_clicked().connect( sigc::mem_fun(*this, &AddTransformation::on_button_remove_transformation) );
  m_rb1.signal_clicked().connect( sigc::mem_fun(*this, &AddTransformation::on_world_rotation_button) );
  m_rb2.signal_clicked().connect( sigc::mem_fun(*this, &AddTransformation::on_own_center_rotation_button) );
  m_rb3.signal_clicked().connect( sigc::mem_fun(*this, &AddTransformation::on_given_coordinate_rotation_button) );

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
  m_vbox.pack_start(main_value_field_a, Gtk::PACK_SHRINK);
  m_vbox.pack_start(main_value_field_b, Gtk::PACK_SHRINK);
  m_vbox.pack_start(main_value_field_c, Gtk::PACK_SHRINK);
  m_vbox.pack_start(button_save_transformation, Gtk::PACK_SHRINK);
  m_vbox.pack_start(button_remove_transformation, Gtk::PACK_SHRINK);
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

  // The `property_max_content_width` is only since gtkmm 3.22, and Moodle uses GTK 3.20
  // LOG(4, "Gtk::ScrolledWindow::property_max_content_width(): %d", m_ScrolledWindow.property_max_content_width());
  m_ScrolledWindow.set_min_content_width(500);

  m_hbox.pack_start(m_ScrolledWindow, true, true);
}

AddTransformation::~AddTransformation()
{
}

Gtk::Window& AddTransformation::getWindow()
{
  return this->window;
}

void AddTransformation::on_button_save_transformation()
{
  int          current_page_index = m_notebook.get_current_page();
  Gtk::Widget* current_page_widget = m_notebook.get_nth_page(current_page_index);

  std::string main_value_a = main_value_field_a.get_text().raw();
  std::string main_value_b = main_value_field_b.get_text().raw();
  std::string main_value_c = main_value_field_c.get_text().raw();

  std::string x_rotation_value  = x_rotation_field.get_text().raw();
  std::string current_page_text = (std::string) m_notebook.get_tab_label_text(*current_page_widget);

  std::string name;
  long double x_rotation{std::stold(x_rotation_value)};

  int x_coord = atoi(main_value_a.c_str());
  int y_coord = atoi(main_value_b.c_str());

  // Currently we do not support 3D, the force the z_coord to be 1
  // int z_coord = atoi(main_value_c.c_str());
  int z_coord = 1;

  if(current_page_text == "Translation")
  {
    name = tfm::format("%s %s %s %s", current_page_text, main_value_a, main_value_c, main_value_c);
    this->transformation.add_translation(name, Coordinate(x_coord, y_coord, z_coord));
  }
  else if(current_page_text == "Rotation")
  {
    name = tfm::format("%s %s %s %s %s %s", current_page_text, x_rotation, this->rotation_type, main_value_a, main_value_b, main_value_c);
    this->transformation.add_rotation(name,
        Array<3, long double>{x_rotation, 0.0, 0.0},
        Coordinate(x_coord, y_coord, z_coord),
        this->rotation_type);
  }
  else if(current_page_text == "Scaling")
  {
    name = tfm::format("%s %s %s %s", current_page_text, main_value_a, main_value_c, main_value_c);
    this->transformation.add_scaling(name, Coordinate(x_coord, y_coord, z_coord));
  }
  else
  {
    LOG(1, "");
    LOG(1, "");
    LOG(1, "ERROR! Current page used: %s", current_page_text);
  }

  LOG(4, "%s", name);
  this->_update_transmations_list();
}

/**
 * Update the list after adding or removing a new item.
 */
void AddTransformation::_update_transmations_list()
{
  m_ListViewText.clear_items();

  for( auto transformation_data : this->transformation.getTransformations() )
  {
    guint row_number = m_ListViewText.append(transformation_data.name);
    m_ListViewText.set_text(row_number, 0, transformation_data.name);
  }
}

void AddTransformation::on_button_remove_transformation()
{
  if( m_ListViewText.size() )
  {
    std::string current_name = (std::string)m_ListViewText.get_text(0);

    this->transformation.remove_transformation(current_name);
    this->_update_transmations_list();
  }
}

void AddTransformation::on_button_close()
{
  this->viewPort.apply(this->object_name, this->transformation);
  this->window.close();
}

void AddTransformation::on_own_center_rotation_button()
{
  this->rotation_type = RotationType::ON_ITS_OWN_CENTER;
}

void AddTransformation::on_world_rotation_button()
{
  this->rotation_type = RotationType::ON_WORLD_CENTER;
}

void AddTransformation::on_given_coordinate_rotation_button()
{
  this->rotation_type = RotationType::ON_GIVEN_COORDINATE;
}

