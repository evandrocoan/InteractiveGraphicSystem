#include "mainwindow.h"

/**
 * https://en.wikipedia.org/wiki/Template:Unicode_chart_Arrows
 *
 * https://www.utf8icons.com/character/9993/envelope
 */
MainWindow::MainWindow() :
      addObject(this->facade),
      addTransformation(this->facade),
      rw_object_service(this->facade),
      button_move_up("↑"),
      button_move_down("↓"),
      button_move_left("←"),
      button_move_right("→"),
      button_rotate_left("↺"),
      button_rotate_right("↻"),
      button_zoom_in("+"),
      button_zoom_out("-"),
      button_add_object("Add"),
      button_delete_object("Rem"),
      button_open_file("Open"),
      button_save_file("Save"),
      main_box(Gtk::ORIENTATION_HORIZONTAL),
      left_box(Gtk::ORIENTATION_VERTICAL),
      left_frame("Controllers"),
      right_frame("DrawingArea")
{
  LOG(2, "Entering...");
  this->main_box.pack_start(left_frame, Gtk::PACK_SHRINK, 10);
  this->left_box.set_border_width(10);
  this->left_frame.add(left_box);

  LOG(4, "Draw options");
  this->setupButtons();
  this->connectButtons();
  this->setDefaultTooltips();

  LOG(4, "DrawingArea");
  DrawingArea& drawingArea = this->facade.drawingArea();
  this->main_box.pack_start(this->right_frame, Gtk::PACK_EXPAND_WIDGET, 10);
  this->right_frame.add(drawingArea);
  drawingArea.show();
  this->facade.addObserver(std::bind(&MainWindow::updateDropdownList, this));

  LOG(4, "Show all components");
  this->window.set_title("CG - Trabalho01 - Evandro S. Coan, Marcello da Silva Klingelfus Junior");
  this->window.set_border_width(15);
  this->window.set_default_size(800, 500);
  this->window.add(this->main_box);
  this->window.show_all_children();

  this->add_test_objects();
}

MainWindow::~MainWindow()
{
}

Gtk::Window& MainWindow::getWindow()
{
  return this->window;
}

void MainWindow::setDefaultTooltips()
{
  button_add_object   .set_tooltip_text("Add new object");
  button_delete_object.set_tooltip_text("Remove current selected object");
  objects_list        .set_tooltip_text("The list of all created objects on the DrawingArea");
  button_rotate_right .set_tooltip_text("Rotate the window to the right");
  button_rotate_left  .set_tooltip_text("Rotate the window to the left");

  entry_move_length   .set_tooltip_text("How many pixels to move the ViewWindow");
  entry_zoom_scale    .set_tooltip_text("A scaling factor as `1.1` or `0.9` to `zoom in` or `zoom out` the ViewWindow");
  button_zoom_in      .set_tooltip_text("Apply the scaling factor to the ViewWindow over the Drawing World");
  button_zoom_out     .set_tooltip_text("Apply inverted the scaling factor to the ViewWindow over the Drawing World");
  button_open_file    .set_tooltip_text("Select a file to load the OBJ file");
  button_save_file    .set_tooltip_text("Select a file to save the current drawing to the OBJ");

  button_move_up      .set_tooltip_text("Move the ViewWindow on the drawing area upwards");
  button_move_down    .set_tooltip_text("Move the ViewWindow on the drawing area downwards");
  button_move_left    .set_tooltip_text("Move the ViewWindow on the drawing area to the left");
  button_move_right   .set_tooltip_text("Move the ViewWindow on the drawing area to the right");
}

void MainWindow::setupButtons()
{
  LOG(4, "Inicializando dado da entrada do tamanho de movimentação");
  entry_move_length.set_width_chars(3);
  entry_move_length.set_text(DEFAULT_MOVE_LENGTH);

  LOG(4, "Inicializando dado da entrada do tamanho da rotaçao");
  entry_rotate_angle.set_width_chars(1);
  entry_rotate_angle.set_text(DEFAULT_ROTATE_ANGLE);

  LOG(4, "Inicializando dado da entrada do tamanho do zoom");
  entry_zoom_scale.set_width_chars(3);
  char array[4];

  sprintf(array, "%d", DEFAULT_ZOOM_SCALE);
  array[3] = '\0';
  entry_zoom_scale.set_text(array);

  LOG(4, "Montando a estrutura da grade de lista de objetos");
  grid_list_obj.set_column_homogeneous(true);
  grid_list_obj.attach(button_add_object,    1, 1, 1, 1);
  grid_list_obj.attach(button_delete_object, 2, 1, 1, 1);
  grid_list_obj.attach(button_open_file,     1, 2, 1, 1);
  grid_list_obj.attach(button_save_file,     2, 2, 1, 1);
  grid_list_obj.attach(objects_list,         1, 3, 2, 1);

  LOG(4, "Adicionando os botões de movimentações na grade de movimentação");
  // grid_move.set_column_homogeneous(true);
  grid_move.attach(button_move_left,  1, 2, 1, 1);
  grid_move.attach(button_move_up,    2, 1, 1, 1);
  grid_move.attach(entry_move_length, 2, 2, 1, 1);
  grid_move.attach(button_move_down,  2, 3, 1, 1);
  grid_move.attach(button_move_right, 3, 2, 1, 1);

  LOG(4, "Adicionando os botões de movimentações na grade de zoom");
  grid_zoom.set_column_homogeneous(true);
  grid_zoom.attach(button_zoom_out,  1, 1, 1, 1);
  grid_zoom.attach(entry_zoom_scale, 2, 1, 1, 1);
  grid_zoom.attach(button_zoom_in,   3, 1, 1, 1);

  LOG(4, "Adicionando os botões de rotaçao na grade de rotaçao");
  grid_rotate.set_column_homogeneous(true);
  grid_rotate.attach(button_rotate_left, 1, 1, 1, 1);
  grid_rotate.attach(entry_rotate_angle, 2, 1, 1, 1);
  grid_rotate.attach(button_rotate_right, 3, 1, 1, 1);

  LOG(4, "Adding the draw options box to left frame");
  left_box.set_border_width(10);
  left_box.set_spacing(10);
  left_box.add(grid_list_obj);
  left_box.add(grid_move);
  left_box.add(grid_zoom);
  left_box.add(grid_rotate);
  left_box.add(this->addTransformation.getBox());
}

void MainWindow::connectButtons()
{
  LOG(4, "Determinando ações quando clicado cada botão;");
  this->objects_list.signal_changed().connect(sigc::mem_fun(*this, &MainWindow::on_objects_list_change));

  this->button_move_up.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_button_move_up));
  this->button_move_down.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_button_move_down));
  this->button_move_left.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_button_move_left));
  this->button_move_right.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_button_move_right));

  this->button_zoom_in.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_button_zoom_in));
  this->button_zoom_out.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_button_zoom_out));

  this->button_rotate_left.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_button_rotate_left));
  this->button_rotate_right.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_button_rotate_right));

  this->button_add_object.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_button_add_object));
  this->button_delete_object.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_button_delete_object));

  this->button_open_file.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_button_open_file));
  this->button_save_file.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_button_save_file));
}

/**
 * Called when the `DrawingArea` objects list is updated.
 */
void MainWindow::updateDropdownList()
{
  LOG(2, "Entering...");
  int added_objects = 0;
  auto objects = this->facade.displayFile().getObjects();

  // LOG(4, "limpa a lista de objetos para reimprimi-la");
  this->objects_list.remove_all();

  for(auto object : objects)
  {
    LOG( 8, "isVisibleOnGui: %s, object: %s", object->isVisibleOnGui(), *object );

    if( object->isVisibleOnGui() )
    {
      added_objects += 1;
      this->objects_list.append(object->getName());
      LOG(4, object->getName().c_str());
    }
  }

  // Set Gtk.ComboBoxText default item?
  // https://stackoverflow.com/questions/14912210/set-gtk-comboboxtext-default-item
  // LOG(4, "Selecting the last item on the ComboBoxText");
  this->objects_list.set_active(added_objects-1);
  this->on_objects_list_change();
}

void MainWindow::on_objects_list_change()
{
  // Also update the name on the `addTransformation` window
  Glib::ustring name = (std::string)objects_list.get_active_text();
  this->addTransformation.object_name = name;
}

void MainWindow::on_button_move_up()
{
  int move_length = atoi(entry_move_length.get_text().raw().c_str());

  if (move_length == 0)
  {
    entry_move_length.set_text(DEFAULT_MOVE_LENGTH);
  }
  else
  {
    this->facade.move(Coordinate(0, move_length, 0));
  }
}

void MainWindow::on_button_move_down()
{
  int move_length = atoi(entry_move_length.get_text().raw().c_str());

  if (move_length == 0)
  {
    entry_move_length.set_text(DEFAULT_MOVE_LENGTH);
  }
  else
  {
    this->facade.move(Coordinate(0, -move_length, 0));
  }
}

void MainWindow::on_button_move_left()
{
  int move_length = atoi(entry_move_length.get_text().raw().c_str());

  if (move_length == 0)
  {
    entry_move_length.set_text(DEFAULT_MOVE_LENGTH);
  }
  else
  {
    this->facade.move(Coordinate(-move_length, 0, 0));
  }
}

void MainWindow::on_button_move_right()
{
  int move_length = atoi(entry_move_length.get_text().raw().c_str());

  if (move_length == 0)
  {
    entry_move_length.set_text(DEFAULT_MOVE_LENGTH);
  }
  else
  {
    this->facade.move(Coordinate(move_length, 0, 0));
  }
}

void MainWindow::on_button_zoom_in()
{
  big_double zoom_scale = atof(entry_zoom_scale.get_text().raw().c_str());

  if (zoom_scale <=1)
  {
    char array[4];
    sprintf(array, "%d", DEFAULT_ZOOM_SCALE);

    array[3] = '\0';
    entry_zoom_scale.set_text(array);
  }
  else
  {
    this->facade.zoom(Coordinate(-zoom_scale, -zoom_scale));
  }
}

void MainWindow::on_button_zoom_out()
{
  float zoom_scale = atof(entry_zoom_scale.get_text().raw().c_str());

  if (zoom_scale <=1)
  {
    char array[4];
    sprintf(array, "%d", DEFAULT_ZOOM_SCALE);

    array[3] = '\0';
    entry_zoom_scale.set_text(array);
  }
  else
  {
    this->facade.zoom(Coordinate(zoom_scale, zoom_scale));
  }
}

void MainWindow::on_button_rotate_left()
{
  big_double rotate_angle = atoi(entry_rotate_angle.get_text().raw().c_str());

  if (rotate_angle == 0)
  {
    entry_move_length.set_text(DEFAULT_ROTATE_ANGLE);
  }
  else
  {
    this->facade.rotate(Coordinate(rotate_angle, 0, 0));
  }
}

void MainWindow::on_button_rotate_right()
{
  big_double rotate_angle = atoi(entry_rotate_angle.get_text().raw().c_str());

  if (rotate_angle == 0)
  {
    entry_move_length.set_text(DEFAULT_ROTATE_ANGLE);
  }
  else
  {
    this->facade.rotate(Coordinate(-rotate_angle, 0, 0));
  }
}

void MainWindow::on_button_add_object()
{
  LOG(2, "Entering...");
  this->addObject.getWindow().show();
}

void MainWindow::on_button_delete_object()
{
  Glib::ustring name = (std::string)objects_list.get_active_text();

  if(!(name.empty()))
  {
    this->facade.removeObject(name);
    this->facade.queue_draw();
  }
}

void MainWindow::on_button_open_file()
{
	choose_file_window = new ChooseFileWindow(Gtk::FILE_CHOOSER_ACTION_OPEN);
	choose_file_window->show();

  std::string file_path = choose_file_window->get_file_path();
	this->rw_object_service.read(file_path);

	this->facade.queue_draw();
  LOG(2, "Successfully opened the objects from file\n");
}

void MainWindow::on_button_save_file()
{
	choose_file_window = new ChooseFileWindow(Gtk::FILE_CHOOSER_ACTION_SAVE);
	choose_file_window->show();

	std::string file_path = choose_file_window->get_file_path();
	this->rw_object_service.write(this->facade.displayFile().getObjects(), file_path);

	LOG(2, "Sucessfull saved the objects on file\n");
}

void MainWindow::add_test_objects()
{
  std::vector<big_double> polygon_coord_list;
  this->rw_object_service.read("./simple_line_polygon_test.obj");
  this->rw_object_service.read("./simple_bezier_polygon.obj");

  // polygon_coord_list.clear();
  // polygon_coord_list.push_back(-50); polygon_coord_list.push_back(-50); polygon_coord_list.push_back(1);
  // polygon_coord_list.push_back(-50); polygon_coord_list.push_back(-30); polygon_coord_list.push_back(1);
  // polygon_coord_list.push_back(-30); polygon_coord_list.push_back(-30); polygon_coord_list.push_back(1);
  // polygon_coord_list.push_back(-30); polygon_coord_list.push_back(-50); polygon_coord_list.push_back(1);
  // this->facade.addPolygon("Polygon", polygon_coord_list, _default_coordinate_value_parameter, _default_coordinate_value_parameter, CurveType::POLYGON);

  // polygon_coord_list.clear();
  // polygon_coord_list.push_back(0); polygon_coord_list.push_back(0); polygon_coord_list.push_back(1);
  // polygon_coord_list.push_back(10); polygon_coord_list.push_back(10); polygon_coord_list.push_back(1);
  // polygon_coord_list.push_back(20); polygon_coord_list.push_back(5); polygon_coord_list.push_back(1);
  // polygon_coord_list.push_back(30); polygon_coord_list.push_back(0); polygon_coord_list.push_back(1);
  // polygon_coord_list.push_back(40); polygon_coord_list.push_back(10); polygon_coord_list.push_back(1);
  // polygon_coord_list.push_back(50); polygon_coord_list.push_back(0); polygon_coord_list.push_back(1);
  // polygon_coord_list.push_back(60); polygon_coord_list.push_back(10); polygon_coord_list.push_back(1);
  // this->facade.addPolygon("Bezier", polygon_coord_list, _default_coordinate_value_parameter, _default_coordinate_value_parameter, CurveType::BEZIER);

  this->facade.queue_draw();
}

