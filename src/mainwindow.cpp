#include "mainwindow.h"

/**
 * https://en.wikipedia.org/wiki/Template:Unicode_chart_Arrows
 *
 * https://www.utf8icons.com/character/9993/envelope
 */
MainWindow::MainWindow() :
      addObject(this->facade),
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
      right_frame("DrawingArea"),

      // Transformation
      liang_barsky_radiobutton("L"),
      cohen_sutheland_radiobutton("C"),
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
  LOG(2, "...");
  this->on_liang_radiobutton();

  // Transformation
  this->on_world_center_rotation_radiobutton();
  this->on_translation_radiobutton();
  this->set_default_values_and_tooltips();
  this->create_scrolling_items_list();
  this->_addThingsToTheWindow();
  this->_connectButtons();

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
  liang_barsky_radiobutton   .set_tooltip_text("Liang-Barsky");
  cohen_sutheland_radiobutton.set_tooltip_text("Cohen-Sutheland");

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

  liang_barsky_radiobutton.set_active();
  liang_barsky_radiobutton.set_halign( Gtk::ALIGN_CENTER );

  cohen_sutheland_radiobutton.join_group(liang_barsky_radiobutton);
  cohen_sutheland_radiobutton.set_halign( Gtk::ALIGN_CENTER );

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
  grid_move.attach(liang_barsky_radiobutton,    1, 1, 1, 1);
  grid_move.attach(button_move_up,              2, 1, 1, 1);
  grid_move.attach(cohen_sutheland_radiobutton, 3, 1, 1, 1);
  grid_move.attach(button_move_left,            1, 2, 1, 1);
  grid_move.attach(entry_move_length,           2, 2, 1, 1);
  grid_move.attach(button_move_right,           3, 2, 1, 1);
  grid_move.attach(button_move_down,            2, 3, 1, 1);

  LOG(4, "Adicionando os botões de movimentações na grade de zoom");
  grid_zoom.set_column_homogeneous(true);
  grid_zoom.attach(button_zoom_out,  1, 1, 1, 1);
  grid_zoom.attach(entry_zoom_scale, 2, 1, 1, 1);
  grid_zoom.attach(button_zoom_in,   3, 1, 1, 1);

  LOG(4, "Adicionando os botões de rotaçao na grade de rotaçao");
  grid_rotate.set_column_homogeneous(true);
  grid_rotate.attach(button_rotate_left,  1, 1, 1, 1);
  grid_rotate.attach(entry_rotate_angle,  2, 1, 1, 1);
  grid_rotate.attach(button_rotate_right, 3, 1, 1, 1);

  LOG(4, "Adding the draw options box to left frame");
  left_box.set_border_width(10);
  left_box.set_spacing(10);
  left_box.add(grid_list_obj);
  left_box.add(grid_move);
  left_box.add(grid_zoom);
  left_box.add(grid_rotate);
  left_box.add(this->main_vertical_box);
}


void MainWindow::connectButtons()
{
  LOG(4, "Determinando ações quando clicado cada botão;");
  this->objects_list.signal_changed().connect(sigc::mem_fun(*this, &MainWindow::on_objects_list_change));
  this->liang_barsky_radiobutton.signal_clicked().connect( sigc::mem_fun(*this, &MainWindow::on_liang_radiobutton) );
  this->cohen_sutheland_radiobutton.signal_clicked().connect( sigc::mem_fun(*this, &MainWindow::on_cohen_radiobutton) );

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
  LOG(2, "...");
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
{ try {

  // Also update the name on the `addTransformation` window
  Glib::ustring name = (std::string)objects_list.get_active_text();
  this->object_name = name;

  } catch( const std::runtime_error& error ) { errorMessage( error ); return; }
}


void MainWindow::on_button_move_up()
{ try {

  int move_length = atoi(entry_move_length.get_text().raw().c_str());

  if (move_length == 0)
  {
    entry_move_length.set_text(DEFAULT_MOVE_LENGTH);
  }
  else
  {
    this->facade.move(Coordinate(0, move_length, 0));
  }

  } catch( const std::runtime_error& error ) { errorMessage( error ); return; }
}


void MainWindow::on_button_move_down()
{ try {

  int move_length = atoi(entry_move_length.get_text().raw().c_str());

  if (move_length == 0)
  {
    entry_move_length.set_text(DEFAULT_MOVE_LENGTH);
  }
  else
  {
    this->facade.move(Coordinate(0, -move_length, 0));
  }

  } catch( const std::runtime_error& error ) { errorMessage( error ); return; }
}


void MainWindow::on_button_move_left()
{ try {

  int move_length = atoi(entry_move_length.get_text().raw().c_str());

  if (move_length == 0)
  {
    entry_move_length.set_text(DEFAULT_MOVE_LENGTH);
  }
  else
  {
    this->facade.move(Coordinate(-move_length, 0, 0));
  }

  } catch( const std::runtime_error& error ) { errorMessage( error ); return; }
}


void MainWindow::on_button_move_right()
{ try {

  int move_length = atoi(entry_move_length.get_text().raw().c_str());

  if (move_length == 0)
  {
    entry_move_length.set_text(DEFAULT_MOVE_LENGTH);
  }
  else
  {
    this->facade.move(Coordinate(move_length, 0, 0));
  }

  } catch( const std::runtime_error& error ) { errorMessage( error ); return; }
}


void MainWindow::on_button_zoom_in()
{ try {

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

  } catch( const std::runtime_error& error ) { errorMessage( error ); return; }
}


void MainWindow::on_button_zoom_out()
{ try {

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

  } catch( const std::runtime_error& error ) { errorMessage( error ); return; }
}


void MainWindow::on_button_rotate_left()
{ try {

  big_double rotate_angle = atoi(entry_rotate_angle.get_text().raw().c_str());

  if (rotate_angle == 0)
  {
    entry_move_length.set_text(DEFAULT_ROTATE_ANGLE);
  }
  else
  {
    this->facade.rotate(Coordinate(rotate_angle, 0, 0));
  }

  } catch( const std::runtime_error& error ) { errorMessage( error ); return; }
}


void MainWindow::on_button_rotate_right()
{ try {

  big_double rotate_angle = atoi(entry_rotate_angle.get_text().raw().c_str());

  if (rotate_angle == 0)
  {
    entry_move_length.set_text(DEFAULT_ROTATE_ANGLE);
  }
  else
  {
    this->facade.rotate(Coordinate(-rotate_angle, 0, 0));
  }

  } catch( const std::runtime_error& error ) { errorMessage( error ); return; }
}


void MainWindow::on_button_add_object()
{ try {

  LOG(2, "...");
  this->addObject.getWindow().show();

  } catch( const std::runtime_error& error ) { errorMessage( error ); return; }
}


void MainWindow::on_button_delete_object()
{ try {

  Glib::ustring name = (std::string)objects_list.get_active_text();

  if(!(name.empty()))
  {
    this->facade.removeObject(name);
    this->facade.queue_draw();
  }

  } catch( const std::runtime_error& error ) { errorMessage( error ); return; }
}


void MainWindow::on_button_open_file()
{ try {

	choose_file_window = new ChooseFileWindow(Gtk::FILE_CHOOSER_ACTION_OPEN);
	choose_file_window->show();

  std::string file_path = choose_file_window->get_file_path();
	this->rw_object_service.read(file_path);

	this->facade.queue_draw();
  LOG(2, "Successfully opened the objects from file\n");

  } catch( const std::runtime_error& error ) { errorMessage( error ); return; }
}


void MainWindow::on_button_save_file()
{ try {

	choose_file_window = new ChooseFileWindow(Gtk::FILE_CHOOSER_ACTION_SAVE);
	choose_file_window->show();

	std::string file_path = choose_file_window->get_file_path();
	this->rw_object_service.write(this->facade.displayFile().getObjects(), file_path);

	LOG(2, "Sucessfull saved the objects on file\n");

  } catch( const std::runtime_error& error ) { errorMessage( error ); return; }
}


void MainWindow::on_liang_radiobutton()
{ try {

  this->facade.setLineClipping( LineClippingType::LIANG_BARSKY );

  } catch( const std::runtime_error& error ) { errorMessage( error ); return; }
}


void MainWindow::on_cohen_radiobutton()
{ try {

  this->facade.setLineClipping( LineClippingType::COHEN_SUTHELAND );

  } catch( const std::runtime_error& error ) { errorMessage( error ); return; }
}


// Transformation
void MainWindow::_connectButtons()
{
  button_apply.signal_clicked().connect( sigc::mem_fun(*this, &MainWindow::on_button_apply) );
  button_save_transformation.signal_clicked().connect( sigc::mem_fun(*this, &MainWindow::on_button_save_transformation) );
  button_remove_transformation.signal_clicked().connect( sigc::mem_fun(*this, &MainWindow::on_button_remove_transformation) );

  world_center_radiobutton.signal_clicked().connect( sigc::mem_fun(*this, &MainWindow::on_world_center_rotation_radiobutton) );
  geometric_center_radiobutton.signal_clicked().connect( sigc::mem_fun(*this, &MainWindow::on_own_center_rotation_radiobutton) );
  any_point_radiobutton.signal_clicked().connect( sigc::mem_fun(*this, &MainWindow::on_given_coordinate_rotation_radiobutton) );
  translation_radiobutton.signal_clicked().connect( sigc::mem_fun(*this, &MainWindow::on_translation_radiobutton) );
  scaling_radiobutton.signal_clicked().connect( sigc::mem_fun(*this, &MainWindow::on_scaling_radiobutton) );
  rotation_radiobutton.signal_clicked().connect( sigc::mem_fun(*this, &MainWindow::on_rotation_radiobutton) );
}


/**
 * how to add text box in gtkmm:gtk::Listbox using c++
 * https://stackoverflow.com/questions/34253297/how-to-add-text-box-in-gtkmmgtklistbox-using-c
 */
void MainWindow::create_scrolling_items_list()
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


void MainWindow::_addThingsToTheWindow()
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
  coodinate_input_grid.attach(y_rotation_field,             2, 2, 1, 1);
  coodinate_input_grid.attach(z_rotation_field,             3, 2, 1, 1);

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


void MainWindow::set_default_values_and_tooltips()
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
  x_rotation_field.set_tooltip_text("Rotation Degrees between 0 and 360 on axis X");

  y_rotation_field.set_text("0");
  y_rotation_field.set_width_chars(3);
  y_rotation_field.set_tooltip_text("Rotation Degrees between 0 and 360 on axis Y");

  z_rotation_field.set_text("0");
  z_rotation_field.set_width_chars(3);
  z_rotation_field.set_tooltip_text("Rotation Degrees between 0 and 360 on axis Z");

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


void MainWindow::on_button_save_transformation()
{ try {

  std::string name;
  std::string x_rotation_value = x_rotation_field.get_text().raw();
  std::string y_rotation_value = y_rotation_field.get_text().raw();
  std::string z_rotation_value = z_rotation_field.get_text().raw();

  std::string main_value_a = main_value_field_a.get_text().raw();
  std::string main_value_b = main_value_field_b.get_text().raw();
  std::string main_value_c = main_value_field_c.get_text().raw();

  big_double x_rotation{std::stold(x_rotation_value)};
  big_double y_rotation{std::stold(y_rotation_value)};
  big_double z_rotation{std::stold(z_rotation_value)};

  big_double x_coord{std::stold(main_value_a.c_str())};
  big_double y_coord{std::stold(main_value_b.c_str())};
  big_double z_coord{std::stold(main_value_c.c_str())};

  if(this->transformation_type == TransformationType::TRANSLATION)
  {
    name = tfm::format("%s %s %s %s", this->transformation_type, main_value_a, main_value_b, main_value_c);
    this->transformation.add_translation(name, Coordinate(x_coord, y_coord, z_coord));
  }
  else if(this->transformation_type == TransformationType::ROTATION)
  {
    name = tfm::format("%s %s %s %s %s %s %s %s",
                       this->transformation_type,
                       x_rotation,
                       y_rotation,
                       z_rotation,
                       this->transformation_point,
                       main_value_a,
                       main_value_b,
                       main_value_c);

    this->transformation.add_rotation(name,
        Coordinate(x_rotation, y_rotation, z_rotation),
        this->transformation_point,
        Coordinate(x_coord, y_coord, z_coord));
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

  } catch( const std::runtime_error& error ) { errorMessage( error ); return; }
}


/**
 * Update the list after adding or removing a new item.
 */
void MainWindow::_update_transmations_list()
{
  list_view_text.clear_items();

  for( auto transformation_data : this->transformation.getTransformations() )
  {
    guint row_number = list_view_text.append(transformation_data.name);
    list_view_text.set_text(row_number, 0, transformation_data.name);
  }
}


void MainWindow::on_button_remove_transformation()
{ try {

  if( list_view_text.size() )
  {
    std::string current_name = (std::string)list_view_text.get_text(0);

    this->transformation.remove_transformation(current_name);
    this->_update_transmations_list();
  }

  } catch( const std::runtime_error& error ) { errorMessage( error ); return; }
}


void MainWindow::on_button_apply()
{ try {

  this->facade.apply(this->object_name, this->transformation);
  this->facade.queue_draw();

  } catch( const std::runtime_error& error ) { errorMessage( error ); return; }
}


void MainWindow::on_translation_radiobutton()
{ try {

  this->transformation_type = TransformationType::TRANSLATION;

  } catch( const std::runtime_error& error ) { errorMessage( error ); return; }
}


void MainWindow::on_scaling_radiobutton()
{ try {

  this->transformation_type = TransformationType::SCALING;

  } catch( const std::runtime_error& error ) { errorMessage( error ); return; }
}


void MainWindow::on_rotation_radiobutton()
{ try {

  this->transformation_type = TransformationType::ROTATION;

  } catch( const std::runtime_error& error ) { errorMessage( error ); return; }
}


void MainWindow::on_own_center_rotation_radiobutton()
{ try {

  this->transformation_point = TransformationPoint::ON_ITS_OWN_CENTER;

  } catch( const std::runtime_error& error ) { errorMessage( error ); return; }
}


void MainWindow::on_world_center_rotation_radiobutton()
{ try {

  this->transformation_point = TransformationPoint::ON_WORLD_CENTER;

  } catch( const std::runtime_error& error ) { errorMessage( error ); return; }
}


void MainWindow::on_given_coordinate_rotation_radiobutton()
{ try {

  this->transformation_point = TransformationPoint::ON_GIVEN_COORDINATE;

  } catch( const std::runtime_error& error ) { errorMessage( error ); return; }
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

  polygon_coord_list.clear();
  polygon_coord_list.push_back(0); polygon_coord_list.push_back(0); polygon_coord_list.push_back(1);
  polygon_coord_list.push_back(10); polygon_coord_list.push_back(10); polygon_coord_list.push_back(1);
  polygon_coord_list.push_back(20); polygon_coord_list.push_back(5); polygon_coord_list.push_back(1);
  polygon_coord_list.push_back(30); polygon_coord_list.push_back(0); polygon_coord_list.push_back(1);
  polygon_coord_list.push_back(40); polygon_coord_list.push_back(10); polygon_coord_list.push_back(1);
  polygon_coord_list.push_back(50); polygon_coord_list.push_back(0); polygon_coord_list.push_back(1);
  polygon_coord_list.push_back(60); polygon_coord_list.push_back(10); polygon_coord_list.push_back(1);
  this->facade.addPolygon("Bezier", polygon_coord_list, _default_coordinate_value_parameter, _default_coordinate_value_parameter, CurveType::BEZIER);

  this->facade.queue_draw();
}
