#include "addobject.h"

AddObject::AddObject(DrawingArea &drawingArea) :
      drawingArea(drawingArea),
      m_vbox(Gtk::ORIENTATION_VERTICAL),
      button_close("Close"),
      button_save_line("Save Line"),
      button_save_wire("Save Polygon"),
      button_save_point("Save Point"),
      button_add_coordenate("Add Coordenate"),
      info_label("Insert a Coordinate :"),
      point_x_label("Coordinate X : "),
      point_y_label("Coordinate Y : "),
      line_x1_label("Coordinate X1 : "),
      line_y1_label("Coordinate Y1 : "),
      line_x2_label("Coordinate X2 : "),
      line_y2_label("Coordinate Y2 : "),
      polygon_x_label("Coordinate X : "),
      polygon_y_label("Coordinate Y : ")
{
  LOG(2, "Entering...");
  point_name_field.set_text("point1");
  point_name_field.set_placeholder_text("Name");
  point_x_field.set_text("50");
  point_y_field.set_text("50");
  point_grid.set_column_homogeneous(true);
  point_grid.set_row_spacing(10);
  point_grid.attach(point_name_field, 1, 1, 2, 1);
  point_grid.attach(point_x_label, 1, 2, 1, 1);
  point_grid.attach(point_x_field, 2, 2, 1, 1);
  point_grid.attach(point_y_label, 1, 3, 1, 1);
  point_grid.attach(point_y_field, 2, 3, 1, 1);
  point_grid.attach(button_save_point, 1, 4, 2, 1);

  line_name_field.set_placeholder_text("Name");
  line_name_field.set_text("line1");
  line_x1_field.set_text("0");
  line_y1_field.set_text("0");
  line_x2_field.set_text("50");
  line_y2_field.set_text("50");
  line_grid.set_column_homogeneous(true);
  line_grid.set_row_spacing(10);
  line_grid.attach(line_name_field, 1, 1, 4, 1);
  line_grid.attach(line_x1_label, 1, 2, 1, 1);
  line_grid.attach(line_x1_field, 2, 2, 1, 1);
  line_grid.attach(line_x2_label, 1, 3, 1, 1);
  line_grid.attach(line_x2_field, 2, 3, 1, 1);
  line_grid.attach(line_y1_label, 3, 2, 1, 1);
  line_grid.attach(line_y1_field, 4, 2, 1, 1);
  line_grid.attach(line_y2_label, 3, 3, 1, 1);
  line_grid.attach(line_y2_field, 4, 3, 1, 1);
  line_grid.attach(button_save_line, 1, 4, 4, 1);

  polygon_name_field.set_placeholder_text("Name");
  polygon_name_field.set_text("polygon1");
  wire_x_field.set_text("0");
  wire_y_field.set_text("0");
  polygn_grid.set_column_homogeneous(true);
  polygn_grid.set_row_spacing(10);
  polygn_grid.attach(polygon_name_field, 1, 1, 1, 1);
  polygn_grid.attach(info_label, 2, 1, 1, 1);
  polygn_grid.attach(polygon_x_label, 1, 2, 1, 1);
  polygn_grid.attach(wire_x_field, 2, 2, 1, 1);
  polygn_grid.attach(polygon_y_label, 1, 3, 1, 1);
  polygn_grid.attach(wire_y_field, 2, 3, 1, 1);
  polygn_grid.attach(button_add_coordenate, 1, 4, 1, 1);
  polygn_grid.attach(button_save_wire, 2, 4, 1, 1);

  button_close.signal_clicked().connect( sigc::mem_fun(*this, &AddObject::on_button_close) );
  button_save_point.signal_clicked().connect( sigc::mem_fun(*this, &AddObject::on_button_save_point) );
  button_save_line.signal_clicked().connect( sigc::mem_fun(*this, &AddObject::on_button_save_line) );
  button_add_coordenate.signal_clicked().connect( sigc::mem_fun(*this, &AddObject::on_button_add_coordinate) );
  button_save_wire.signal_clicked().connect( sigc::mem_fun(*this, &AddObject::on_button_save_polygon) );

  m_notebook.set_border_width(0);
  m_vbox.pack_start(m_notebook);
  m_vbox.pack_start(button_close, Gtk::PACK_SHRINK);

  m_notebook.append_page(point_grid, "Point");
  m_notebook.append_page(line_grid, "Line");
  m_notebook.append_page(polygn_grid, "Polygon");

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
{
  std::string name = point_name_field.get_text().raw();
  LOG(4, "Name: %s", name);

  if (name.empty())
  {
    point_name_field.grab_focus();
    return;
  }

  std::string x_string = point_x_field.get_text().raw();
  std::string y_string = point_y_field.get_text().raw();

  int x_coord = atoi(x_string.c_str());
  int y_coord = atoi(y_string.c_str());

  this->drawingArea.addPoint(name, x_coord, y_coord);
  this->drawingArea.queue_draw();

  this->window.close();
}

void AddObject::on_button_save_line()
{
  std::string name = line_name_field.get_text().raw();
  LOG(4, "Name: %s", name);

  if (name.empty())
  {
    line_name_field.grab_focus();
    return;
  }

  std::string x1_string = line_x1_field.get_text().raw();
  std::string y1_string = line_y1_field.get_text().raw();
  std::string x2_string = line_x2_field.get_text().raw();
  std::string y2_string = line_y2_field.get_text().raw();

  int x1_cord = atoi(x1_string.c_str());
  int y1_cord = atoi(y1_string.c_str());
  int x2_cord = atoi(x2_string.c_str());
  int y2_cord = atoi(y2_string.c_str());

  this->drawingArea.addLine(name, x1_cord, y1_cord, x2_cord, y2_cord);
  this->drawingArea.queue_draw();

  this->window.close();
}

void AddObject::on_button_save_polygon()
{
  if (!polygon_coord_list.empty())
  {
    std::string name = polygon_name_field.get_text().raw();
    LOG(4, "Name: %s", name);

    if (name.empty())
    {
      polygon_name_field.grab_focus();
      return;
    }

    this->drawingArea.addPolygon(name, polygon_coord_list);
    this->drawingArea.queue_draw();

    while(!polygon_coord_list.empty())
    {
      polygon_coord_list.pop_back();
    }

    this->window.close();
  }
  else
  {
    return ;
  }
}

void AddObject::on_button_add_coordinate()
{
  std::string x_string = wire_x_field.get_text().raw();
  std::string y_string = wire_y_field.get_text().raw();
  // std::string z_string = wire_z_field.get_text().raw();

  int x_coord = atoi(x_string.c_str());
  int y_coord = atoi(y_string.c_str());
  // int z_coord = atoi(z_string.c_str());

  polygon_coord_list.push_back(x_coord);
  polygon_coord_list.push_back(y_coord);
  // polygon_coord_list.push_back(z_coord);
  polygon_coord_list.push_back(1);

  wire_x_field.set_text("");
  wire_y_field.set_text("");

  std::string info_label_contents = "Added X : " + std::to_string(x_coord) + " Y : " + std::to_string(y_coord);

  LOG(4, info_label_contents.c_str());
  info_label.set_text(info_label_contents);
}

void AddObject::on_button_close()
{
  this->window.close();
}

