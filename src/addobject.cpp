#include "addobject.h"

AddObject::AddObject(Facade &facade) :
      facade(facade),
      m_vbox(Gtk::ORIENTATION_VERTICAL),
      button_close("Close"),
      button_save_line("Save Line"),
      button_save_polygon("Save Polygon"),
      button_save_bezier("Save Bezier Curve"),
      button_save_bspline("Save B-Spline Curve"),
      button_save_point("Save Point"),
      button_add_coordenate("Add Coordenate"),
      button_remove_coordenate("Remove Coordenate"),
      insert_border_color_label("Insert a RGB border color: "),
      insert_filling_color_label("Insert a RGB filling color: "),
      polygon_x_label("Coordinate X: "),
      polygon_y_label("Coordinate Y: "),
      polygon_z_label("Coordinate Z: ")
{
  LOG(2, "...");

  entered_points_field.set_text("Added Coordinates: ");
  entered_points_field.set_line_wrap(true);
  entered_points_field.set_size_request(250, -1);
  entered_points_field.set_xalign(0);
  entered_points_field.set_yalign(0);
  entered_points_field.set_max_width_chars(50);

  insert_border_color_label.set_xalign(0);
  insert_border_color_label.set_yalign(0.5);
  insert_filling_color_label.set_xalign(0);
  insert_filling_color_label.set_yalign(0.5);

  insert_border_color_field_r.set_text("0");
  insert_border_color_field_g.set_text("0");
  insert_border_color_field_b.set_text("0");
  insert_filling_color_field_r.set_text("0");
  insert_filling_color_field_g.set_text("0");
  insert_filling_color_field_b.set_text("0");
  color_grid.attach(insert_border_color_label   , 1, 1, 1, 1);
  color_grid.attach(insert_border_color_field_r , 2, 1, 1, 1);
  color_grid.attach(insert_border_color_field_g , 3, 1, 1, 1);
  color_grid.attach(insert_border_color_field_b , 4, 1, 1, 1);
  color_grid.attach(insert_filling_color_label  , 1, 2, 1, 1);
  color_grid.attach(insert_filling_color_field_r, 2, 2, 1, 1);
  color_grid.attach(insert_filling_color_field_g, 3, 2, 1, 1);
  color_grid.attach(insert_filling_color_field_b, 4, 2, 1, 1);

  polygon_name_field.set_placeholder_text("Name");
  polygon_name_field.set_text("polygon");
  wire_x_field.set_text("0");
  wire_y_field.set_text("0");
  wire_z_field.set_text("0");
  polygon_grid.set_column_homogeneous(true);
  polygon_grid.set_row_spacing(10);
  polygon_grid.attach(polygon_name_field       , 1, 1, 1, 1);
  polygon_grid.attach(polygon_x_label          , 2, 1, 1, 1);
  polygon_grid.attach(wire_x_field             , 3, 1, 1, 1);
  polygon_grid.attach(polygon_y_label          , 2, 2, 1, 1);
  polygon_grid.attach(wire_y_field             , 3, 2, 1, 1);
  polygon_grid.attach(polygon_z_label          , 2, 3, 1, 1);
  polygon_grid.attach(wire_z_field             , 3, 3, 1, 1);
  polygon_grid.attach(button_add_coordenate    , 1, 4, 1, 1);
  polygon_grid.attach(button_close             , 2, 4, 1, 1);
  polygon_grid.attach(button_remove_coordenate , 3, 4, 1, 1);
  polygon_grid.attach(button_save_point        , 1, 6, 1, 1);
  polygon_grid.attach(button_save_line         , 2, 6, 1, 1);
  polygon_grid.attach(button_save_polygon      , 3, 6, 1, 1);
  polygon_grid.attach(button_save_bezier       , 1, 7, 1, 1);
  polygon_grid.attach(button_save_bspline      , 2, 7, 1, 1);

  button_close.signal_clicked().connect( sigc::mem_fun(*this, &AddObject::on_button_close) );
  button_save_point.signal_clicked().connect( sigc::mem_fun(*this, &AddObject::on_button_save_point) );
  button_save_line.signal_clicked().connect( sigc::mem_fun(*this, &AddObject::on_button_save_line) );
  button_add_coordenate.signal_clicked().connect( sigc::mem_fun(*this, &AddObject::on_button_add_coordinate) );
  button_remove_coordenate.signal_clicked().connect( sigc::mem_fun(*this, &AddObject::on_button_remove_coordinate) );
  button_save_polygon.signal_clicked().connect( sigc::mem_fun(*this, &AddObject::on_button_save_polygon) );
  button_save_bezier.signal_clicked().connect( sigc::mem_fun(*this, &AddObject::on_button_save_bezier) );
  button_save_bspline.signal_clicked().connect( sigc::mem_fun(*this, &AddObject::on_button_save_bspline) );

  m_vbox.pack_start(polygon_grid, Gtk::PACK_SHRINK);
  m_vbox.pack_start(color_grid, Gtk::PACK_SHRINK);
  m_vbox.pack_start(entered_points_field, Gtk::PACK_SHRINK);

  this->window.set_title("Add Object");
  this->window.set_border_width(12);
  this->window.add(m_vbox);
  this->window.show_all_children();
}

AddObject::~AddObject()
{
}

Gtk::Window& AddObject::getWindow()
{
  return this->window;
}

void AddObject::on_button_save_point()
{ try {

  std::string name = this->_get_field_name(polygon_name_field);
  LOG(4, "Name: %s", name);

  if (name.empty())
  {
    polygon_name_field.grab_focus();
    return;
  }

  if( polygon_coord_list.size() < 3 )
  {
    std::ostringstream contents;
    for( auto value : polygon_coord_list ) contents << value << ", ";

    std::string error = tfm::format(
        "A line curve should have 1 coordinate, not %s.\n\n%s",
        polygon_coord_list.size() / 3, contents.str() );

    throw std::runtime_error( error );
  }

  int z_coord = polygon_coord_list.back(); polygon_coord_list.pop_back();
  int y_coord = polygon_coord_list.back(); polygon_coord_list.pop_back();
  int x_coord = polygon_coord_list.back(); polygon_coord_list.pop_back();

  Coordinate border = this->_get_rgb_color(insert_border_color_field_r, insert_border_color_field_g, insert_border_color_field_b);

  this->facade.addPoint(name, x_coord, y_coord, z_coord, border);
  this->facade.queue_draw();
  this->window.close();

  } catch( const std::runtime_error& error ) { errorMessage( error ); return; }
}

std::string AddObject::_get_field_name(Gtk::Entry &name_field)
{
  static int linha_count = 1;
  std::string name = name_field.get_text().raw();
  name = name + std::to_string(linha_count);
  linha_count++;
  return name;
}

void AddObject::on_button_save_line()
{ try {

  std::string name = this->_get_field_name(polygon_name_field);
  LOG(4, "Name: %s", name);

  if (name.empty())
  {
    polygon_name_field.grab_focus();
    return;
  }

  if( polygon_coord_list.size() < 6 )
  {
    std::ostringstream contents;
    for( auto value : polygon_coord_list ) contents << value << ", ";

    std::string error = tfm::format(
        "A line curve should have 2 coordinates, not %s.\n\n%s",
        polygon_coord_list.size() / 3, contents.str() );

    throw std::runtime_error( error );
  }

  int z1 = polygon_coord_list.back(); polygon_coord_list.pop_back();
  int y1 = polygon_coord_list.back(); polygon_coord_list.pop_back();
  int x1 = polygon_coord_list.back(); polygon_coord_list.pop_back();

  int z2 = polygon_coord_list.back(); polygon_coord_list.pop_back();
  int y2 = polygon_coord_list.back(); polygon_coord_list.pop_back();
  int x2 = polygon_coord_list.back(); polygon_coord_list.pop_back();

  Coordinate border = this->_get_rgb_color(insert_border_color_field_r, insert_border_color_field_g, insert_border_color_field_b);

  this->facade.addLine(name, x1, y1, z1, x2, y2, z2, border);
  this->facade.queue_draw();
  this->window.close();

  } catch( const std::runtime_error& error ) { errorMessage( error ); return; }
}

void AddObject::on_button_save_polygon()
{ try {

  std::string name = this->_get_field_name(polygon_name_field);
  LOG(4, "Name: %s", name);

  if (name.empty())
  {
    polygon_name_field.grab_focus();
    return;
  }

  if( polygon_coord_list.size() < 9 )
  {
    std::ostringstream contents;
    for( auto value : polygon_coord_list ) contents << value << ", ";

    std::string error = tfm::format(
        "A line curve should have at least 3 coordinates, not %s.\n\n%s",
        polygon_coord_list.size() / 3, contents.str() );

    throw std::runtime_error( error );
  }

  Coordinate border = this->_get_rgb_color(insert_border_color_field_r, insert_border_color_field_g, insert_border_color_field_b);
  Coordinate filling = this->_get_rgb_color(insert_filling_color_field_r, insert_filling_color_field_g, insert_filling_color_field_b);

  this->facade.addPolygon(name, polygon_coord_list, border, filling);
  this->facade.queue_draw();

  while(!polygon_coord_list.empty())
  {
    polygon_coord_list.pop_back();
  }

  entered_points_field.set_text("Added Coordinates: ");
  entered_points_text.clear();
  this->window.close();

  } catch( const std::runtime_error& error ) { errorMessage( error ); return; }
}


void AddObject::on_button_save_bezier()
{ try {

  std::string name = this->_get_field_name(polygon_name_field);
  LOG(4, "Name: %s", name);

  if (name.empty())
  {
    polygon_name_field.grab_focus();
    return;
  }

  Coordinate border = this->_get_rgb_color(insert_border_color_field_r, insert_border_color_field_g, insert_border_color_field_b);
  Coordinate filling = this->_get_rgb_color(insert_filling_color_field_r, insert_filling_color_field_g, insert_filling_color_field_b);

  this->facade.addPolygon(name, polygon_coord_list, border, filling, CurveType::BEZIER);
  this->facade.queue_draw();

  while(!polygon_coord_list.empty())
  {
    polygon_coord_list.pop_back();
  }

  entered_points_field.set_text("Added Coordinates: ");
  entered_points_text.clear();
  this->window.close();

  } catch( const std::runtime_error& error ) { errorMessage( error ); return; }
}


void AddObject::on_button_save_bspline()
{ try {

  std::string name = this->_get_field_name(polygon_name_field);
  LOG(4, "Name: %s", name);

  if (name.empty())
  {
    polygon_name_field.grab_focus();
    return;
  }

  Coordinate border = this->_get_rgb_color(insert_border_color_field_r, insert_border_color_field_g, insert_border_color_field_b);
  Coordinate filling = this->_get_rgb_color(insert_filling_color_field_r, insert_filling_color_field_g, insert_filling_color_field_b);

  this->facade.addPolygon(name, polygon_coord_list, border, filling, CurveType::BSPLINE);
  this->facade.queue_draw();

  while(!polygon_coord_list.empty())
  {
    polygon_coord_list.pop_back();
  }

  entered_points_field.set_text("Added Coordinates: ");
  entered_points_text.clear();
  this->window.close();

  } catch( const std::runtime_error& error ) { errorMessage( error ); return; }
}


void AddObject::on_button_add_coordinate()
{ try {

  std::string x_string = wire_x_field.get_text().raw();
  std::string y_string = wire_y_field.get_text().raw();
  std::string z_string = wire_z_field.get_text().raw();

  int x_coord = atoi(x_string.c_str());
  int y_coord = atoi(y_string.c_str());
  int z_coord = atoi(z_string.c_str());

  polygon_coord_list.push_back(x_coord);
  polygon_coord_list.push_back(y_coord);
  polygon_coord_list.push_back(z_coord);

  wire_x_field.set_text("");
  wire_y_field.set_text("");
  wire_z_field.set_text("");

  entered_points_text.push_back( "("
      + std::to_string(x_coord) + ", "
      + std::to_string(y_coord) + ", "
      + std::to_string(z_coord) + ")" );

  std::ostringstream contents;

  for( auto value : entered_points_text ) contents << value << ", ";
  entered_points_field.set_text( "Added Coordinates: " + contents.str() );

  } catch( const std::runtime_error& error ) { errorMessage( error ); return; }
}


void AddObject::on_button_remove_coordinate()
{ try {

  polygon_coord_list.pop_back();
  polygon_coord_list.pop_back();
  polygon_coord_list.pop_back();

  entered_points_text.pop_back();
  std::ostringstream contents;

  for( auto value : entered_points_text ) contents << value << ", ";
  entered_points_field.set_text( "Added Coordinates: " + contents.str() );

  } catch( const std::runtime_error& error ) { errorMessage( error ); return; }
}


void AddObject::on_button_close()
{ try {

  this->window.close();

  } catch( const std::runtime_error& error ) { errorMessage( error ); return; }
}


Coordinate AddObject::_get_rgb_color(Gtk::Entry& field_r, Gtk::Entry& field_g, Gtk::Entry& field_b)
{
  big_double border_r{std::stold(field_r.get_text().raw().c_str())};
  big_double border_g{std::stold(field_g.get_text().raw().c_str())};
  big_double border_b{std::stold(field_b.get_text().raw().c_str())};
  Coordinate border = Coordinate(border_r, border_g, border_b);

  if( border + 0.0001 < 0.0 || border - 0.0001 > 1.0 )
  {
    LOG(1, "");
    LOG(1, "");
    LOG(1, "Error! Invalid RGB color passed for border or filling: %s", border);
    return Coordinate(0, 0, 0);
  }

  return border;
}

