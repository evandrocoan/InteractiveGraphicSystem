#include "addtransformation.h"

AddTransformation::AddTransformation(DrawingArea &drawingArea) :
      drawingArea(drawingArea),
      main_vertical_box(Gtk::ORIENTATION_VERTICAL),
      list_view_text(1),
      translation_radiobutton("T"),
      scaling_radiobutton("S"),
      rotation_radiobutton("R"),
      world_center_radiobutton("WC"),
      geometric_center_radiobutton("GC"),
      any_point_radiobutton("AP"),
      button_apply("A"),
      button_save_transformation("S"),
      button_remove_transformation("R")
{
  LOG(2, "Entering...");
  this->on_world_center_rotation_radiobutton();
  this->on_translation_radiobutton();

  this->set_default_values_and_tooltips();
  this->create_scrolling_items_list();
  this->_addThingsToTheWindow();
  this->_connectButtons();
}

void AddTransformation::_connectButtons()
{
  button_apply.signal_clicked().connect( sigc::mem_fun(*this, &AddTransformation::on_button_apply) );
  button_save_transformation.signal_clicked().connect( sigc::mem_fun(*this, &AddTransformation::on_button_save_transformation) );
  button_remove_transformation.signal_clicked().connect( sigc::mem_fun(*this, &AddTransformation::on_button_remove_transformation) );

  world_center_radiobutton.signal_clicked().connect( sigc::mem_fun(*this, &AddTransformation::on_world_center_rotation_radiobutton) );
  geometric_center_radiobutton.signal_clicked().connect( sigc::mem_fun(*this, &AddTransformation::on_own_center_rotation_radiobutton) );
  any_point_radiobutton.signal_clicked().connect( sigc::mem_fun(*this, &AddTransformation::on_given_coordinate_rotation_radiobutton) );
  translation_radiobutton.signal_clicked().connect( sigc::mem_fun(*this, &AddTransformation::on_translation_radiobutton) );
  scaling_radiobutton.signal_clicked().connect( sigc::mem_fun(*this, &AddTransformation::on_scaling_radiobutton) );
  rotation_radiobutton.signal_clicked().connect( sigc::mem_fun(*this, &AddTransformation::on_rotation_radiobutton) );
}

/**
 * how to add text box in gtkmm:gtk::Listbox using c++
 * https://stackoverflow.com/questions/34253297/how-to-add-text-box-in-gtkmmgtklistbox-using-c
 */
void AddTransformation::create_scrolling_items_list()
{
  //Fill the list_view_text:
  list_view_text.set_column_title(0, "Transformations");

  //Add the TreeView, inside a ScrolledWindow, with the button underneath
  scrolled_window.add(list_view_text);

  //Only show the scrollbars when they are necessary
  scrolled_window.set_policy(Gtk::PolicyType::POLICY_AUTOMATIC, Gtk::PolicyType::POLICY_AUTOMATIC);

  // The `property_max_content_width` is only since gtkmm 3.22, and Moodle uses GTK 3.20
  // LOG(4, "Gtk::ScrolledWindow::property_max_content_width(): %d", scrolled_window.property_max_content_width());
  scrolled_window.set_min_content_width(50);
}

void AddTransformation::_addThingsToTheWindow()
{
  translation_radiobutton.set_active();
  scaling_radiobutton.join_group(translation_radiobutton);
  rotation_radiobutton.join_group(translation_radiobutton);

  world_center_radiobutton.set_active();
  geometric_center_radiobutton.join_group(world_center_radiobutton);
  any_point_radiobutton.join_group(world_center_radiobutton);

  coodinate_input_grid.attach(world_center_radiobutton,     1, 1, 1, 1);
  coodinate_input_grid.attach(geometric_center_radiobutton, 2, 1, 1, 1);
  coodinate_input_grid.attach(any_point_radiobutton,        3, 1, 1, 1);
  coodinate_input_grid.attach(x_rotation_field,             1, 2, 1, 1);

  coodinate_input_grid.attach(translation_radiobutton,      1, 3, 1, 1);
  coodinate_input_grid.attach(scaling_radiobutton,          2, 3, 1, 1);
  coodinate_input_grid.attach(rotation_radiobutton,         3, 3, 1, 1);
  coodinate_input_grid.attach(main_value_field_a,           1, 4, 1, 1);
  coodinate_input_grid.attach(main_value_field_b,           2, 4, 1, 1);
  coodinate_input_grid.attach(main_value_field_c,           3, 4, 1, 1);
  coodinate_input_grid.attach(button_apply,                 1, 5, 1, 1);
  coodinate_input_grid.attach(button_save_transformation,   2, 5, 1, 1);
  coodinate_input_grid.attach(button_remove_transformation, 3, 5, 1, 1);

  main_vertical_box.pack_start(coodinate_input_grid, Gtk::PACK_SHRINK);
  main_vertical_box.pack_start(scrolled_window, true, true);
}

void AddTransformation::set_default_values_and_tooltips()
{
  world_center_radiobutton     .set_tooltip_text("Rotate the object around the World Center");
  geometric_center_radiobutton .set_tooltip_text("Rotate around the object own Geometric Center" );
  any_point_radiobutton        .set_tooltip_text("Rotate the object around Any given Point");

  button_apply                 .set_tooltip_text("Apply the listed transformations to the selected object");
  button_remove_transformation .set_tooltip_text("Remove the current transformation from the transformations list");
  button_save_transformation   .set_tooltip_text("Add the current transformation to the transformations list");

  translation_radiobutton      .set_tooltip_text("Select this to add Translation");
  scaling_radiobutton          .set_tooltip_text("Select this to add Scaling");
  rotation_radiobutton         .set_tooltip_text("Select this to add Rotation");

  x_rotation_field.set_text("15");
  x_rotation_field.set_width_chars(3);
  x_rotation_field.set_tooltip_text("Rotation Degrees between 0 and 360");

  main_value_field_a.set_text("10");
  main_value_field_a.set_width_chars(3);
  main_value_field_a.set_tooltip_text("The X coordinate");

  main_value_field_b.set_text("1");
  main_value_field_b.set_width_chars(3);
  main_value_field_b.set_tooltip_text("The Y coordinate");

  main_value_field_c.set_text("1");
  main_value_field_c.set_width_chars(3);
  main_value_field_c.set_tooltip_text("The Z coordinate");
}

AddTransformation::~AddTransformation()
{
}

Gtk::Box& AddTransformation::getBox()
{
  return this->main_vertical_box;
}

void AddTransformation::on_button_save_transformation()
{
  std::string name;
  std::string x_rotation_value = x_rotation_field.get_text().raw();

  std::string main_value_a = main_value_field_a.get_text().raw();
  std::string main_value_b = main_value_field_b.get_text().raw();
  std::string main_value_c = main_value_field_c.get_text().raw();

  COORDINATE_TYPE x_rotation{std::stold(x_rotation_value)};
  COORDINATE_TYPE x_coord{std::stold(main_value_a.c_str())};
  COORDINATE_TYPE y_coord{std::stold(main_value_b.c_str())};
  COORDINATE_TYPE z_coord{std::stold(main_value_c.c_str())};

  if(this->transformation_type == TransformationType::TRANSLATION)
  {
    name = tfm::format("%s %s %s %s", this->transformation_type, main_value_a, main_value_b, main_value_c);
    this->transformation.add_translation(name, Coordinate(x_coord, y_coord, z_coord));
  }
  else if(this->transformation_type == TransformationType::ROTATION)
  {
    name = tfm::format("%s %s %s %s %s %s",
                       this->transformation_type,
                       x_rotation,
                       this->transformation_point,
                       main_value_a,
                       main_value_b,
                       main_value_c);

    this->transformation.add_rotation(name,
        Coordinate(x_rotation, 0.0, 0.0),
        Coordinate(x_coord, y_coord, z_coord),
        this->transformation_point);
  }
  else if(this->transformation_type == TransformationType::SCALING)
  {
    name = tfm::format("%s %s %s %s", this->transformation_type, main_value_a, main_value_b, main_value_c);
    this->transformation.add_scaling(name, Coordinate(x_coord, y_coord, z_coord));
  }
  else
  {
    LOG(1, "");
    LOG(1, "");
    LOG(1, "ERROR! Invalid transformation type used: %s", transformation_type);
  }

  LOG(4, "%s", name);
  this->_update_transmations_list();
}

/**
 * Update the list after adding or removing a new item.
 */
void AddTransformation::_update_transmations_list()
{
  list_view_text.clear_items();

  for( auto transformation_data : this->transformation.getTransformations() )
  {
    guint row_number = list_view_text.append(transformation_data.name);
    list_view_text.set_text(row_number, 0, transformation_data.name);
  }
}

void AddTransformation::on_button_remove_transformation()
{
  if( list_view_text.size() )
  {
    std::string current_name = (std::string)list_view_text.get_text(0);

    this->transformation.remove_transformation(current_name);
    this->_update_transmations_list();
  }
}

void AddTransformation::on_button_apply()
{
  this->drawingArea.apply(this->object_name, this->transformation);
  this->drawingArea.queue_draw();
}

void AddTransformation::on_translation_radiobutton()
{
  this->transformation_type = TransformationType::TRANSLATION;
}

void AddTransformation::on_scaling_radiobutton()
{
  this->transformation_type = TransformationType::SCALING;
}

void AddTransformation::on_rotation_radiobutton()
{
  this->transformation_type = TransformationType::ROTATION;
}

void AddTransformation::on_own_center_rotation_radiobutton()
{
  this->transformation_point = TransformationPoint::ON_ITS_OWN_CENTER;
}

void AddTransformation::on_world_center_rotation_radiobutton()
{
  this->transformation_point = TransformationPoint::ON_WORLD_CENTER;
}

void AddTransformation::on_given_coordinate_rotation_radiobutton()
{
  this->transformation_point = TransformationPoint::ON_GIVEN_COORDINATE;
}

