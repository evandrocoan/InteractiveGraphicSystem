#include "mainwindow.h"

MainWindow::MainWindow() :
      addObject(this->viewPort),
      addTransformation(this->viewPort),
      button_move_up("up"),
      button_move_down("down"),
      button_move_left("left"),
      button_move_right("right"),
      button_rotate_left("left"),
      button_rotate_right("right"),
      button_zoom_in("+"),
      button_zoom_out("-"),
      button_add_object("Add Object"),
      button_delete_object("Delete Object"),
      main_box(Gtk::ORIENTATION_HORIZONTAL),
      left_box(Gtk::ORIENTATION_VERTICAL),
      left_frame("Controllers"),
      right_frame("ViewPort"),
      button_move_center("Center with the world")
{
  LOG(2, "Entering...");
  this->main_box.pack_start(left_frame, Gtk::PACK_SHRINK, 10);
  this->left_box.set_border_width(10);
  this->left_frame.add(left_box);

  LOG(4, "Draw options");
  this->setupButtons();
  this->connectButtons();

  LOG(4, "ViewPort");
  this->main_box.pack_start(this->right_frame, Gtk::PACK_EXPAND_WIDGET, 10);
  this->right_frame.add(this->viewPort);
  this->viewPort.show();
  this->viewPort.addObserver(std::bind(&MainWindow::updateDropdownList, this));

  LOG(4, "Show all components");
  this->window.set_title("CG - Trabalho01 - Karla Ap. Justen, Evandro S. Coan, Hugo Vincent");
  this->window.set_border_width(15);
  this->window.set_default_size(800, 500);
  this->window.add(this->main_box);
  this->window.show_all_children();
}

MainWindow::~MainWindow()
{
}

Gtk::Window& MainWindow::getWindow()
{
  return this->window;
}

void MainWindow::setupButtons()
{
  LOG(4, "Inicializando dado da entrada do tamanho de movimentação");
  entry_move_length.set_width_chars(1);
  entry_move_length.set_text(DEFAULT_MOVE_LENGTH);

  LOG(4, "Inicializando dado da entrada do tamanho da rotaçao");
  entry_rotate_angle.set_width_chars(1);
  entry_rotate_angle.set_text(DEFAULT_ROTATE_ANGLE);

  LOG(4, "Inicializando dado da entrada do tamanho do zoom");
  entry_zoom_scale.set_width_chars(1);
  char array[4];

  sprintf(array, "%f", DEFAULT_ZOOM_SCALE);
  array[3] = '\0';
  entry_zoom_scale.set_text(array);

  LOG(4, "Montando a estrutura da grade de lista de objetos");
  grid_list_obj.set_column_homogeneous(true);
  grid_list_obj.attach(button_add_object, 1, 1, 1, 1);
  grid_list_obj.attach(objects_list, 1, 2, 1, 1);
  grid_list_obj.attach(button_delete_object, 1, 3, 1, 1);

  LOG(4, "Adicionando os botões de movimentações na grade de movimentação");
  grid_move.set_column_homogeneous(true);
  grid_move.attach(button_move_up, 2, 1, 1, 1);
  grid_move.attach(button_move_left, 1, 2, 1, 1);
  grid_move.attach(entry_move_length, 2, 2, 1, 1);
  grid_move.attach(button_move_right, 3, 2, 1, 1);
  grid_move.attach(button_move_down, 2, 3, 1, 1);

  LOG(4, "Adicionando os botões de movimentações na grade de zoom");
  grid_zoom.set_column_homogeneous(true);
  grid_zoom.attach(button_zoom_out, 1, 1, 1, 1);
  grid_zoom.attach(entry_zoom_scale, 2, 1, 1, 1);
  grid_zoom.attach(button_zoom_in, 3, 1, 1, 1);

  LOG(4, "Adicionando os botões de rotaçao na grade de rotaçao");
  grid_rotate.set_column_homogeneous(true);
  grid_rotate.attach(button_rotate_left, 1, 1, 1, 1);
  grid_rotate.attach(entry_rotate_angle, 2, 1, 1, 1);
  grid_rotate.attach(button_rotate_right, 3, 1, 1, 1);

  LOG(4, "Adding the other button");
  grid_other.set_column_homogeneous(true);
  grid_other.attach(button_move_center, 1, 1, 1, 1);

  LOG(4, "Adding the draw options box to left frame");
  left_box.set_border_width(10);
  left_box.set_spacing(10);
  left_box.add(grid_list_obj);
  left_box.add(grid_move);
  left_box.add(grid_zoom);
  left_box.add(grid_rotate);
  left_box.add(grid_other);
  left_box.add(this->addTransformation.getBox());

 
}

void MainWindow::connectButtons()
{
  LOG(4, "Determinando ações quando clicado cada botão;");
  this->button_move_up.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_button_move_up));
  this->button_move_down.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_button_move_down));
  this->button_move_left.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_button_move_left));
  this->button_move_right.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_button_move_right));

  this->button_move_center.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_button_move_center));

  this->button_zoom_in.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_button_zoom_in));
  this->button_zoom_out.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_button_zoom_out));

  this->button_rotate_left.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_button_rotate_left));
  this->button_rotate_right.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_button_rotate_right));

  this->button_add_object.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_button_add_object));
  this->button_delete_object.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_button_delete_object));
  
}

/**
 * Called when the `ViewPort` objects list is updated.
 */
void MainWindow::updateDropdownList()
{
  LOG(2, "Entering...");
  auto names = this->viewPort.getNamesList();

  LOG(4, "limpa a lista de objetos para reimprimi-la");
  this->objects_list.remove_all();

  for(auto object : names)
  {
    this->objects_list.append(object);
    LOG(4, object.c_str());
  }

  // Set Gtk.ComboBoxText default item?
  // https://stackoverflow.com/questions/14912210/set-gtk-comboboxtext-default-item
  LOG(4, "Selecting the last item on the ComboBoxText");
  this->objects_list.set_active(names.size()-1);

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
    this->viewPort.move_up(move_length);
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
    this->viewPort.move_down(move_length);
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
    this->viewPort.move_left(move_length);
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
    this->viewPort.move_right(move_length);
  }
}

void MainWindow::on_button_move_center()
{
 
  
  this->viewPort.move_center();
  
}

void MainWindow::on_button_zoom_in()
{
  float zoom_scale = atof(entry_zoom_scale.get_text().raw().c_str());

  if (zoom_scale <=1)
  {
    char array[4];
    sprintf(array, "%f", DEFAULT_ZOOM_SCALE);

    array[3] = '\0';
    entry_zoom_scale.set_text(array);
  }
  else
  {
    this->viewPort.zoom_in(zoom_scale);
  }
}

void MainWindow::on_button_zoom_out()
{
  float zoom_scale = atof(entry_zoom_scale.get_text().raw().c_str());

  if (zoom_scale <=1)
  {
    char array[4];
    sprintf(array, "%f", DEFAULT_ZOOM_SCALE);

    array[3] = '\0';
    entry_zoom_scale.set_text(array);
  }
  else
  {
    this->viewPort.zoom_out(zoom_scale);
  }
}

void MainWindow::on_button_rotate_left()
{
  long double rotate_angle = atoi(entry_rotate_angle.get_text().raw().c_str());

  if (rotate_angle == 0)
  {
    entry_move_length.set_text(DEFAULT_ROTATE_ANGLE);
  }
  else
  {
    this->viewPort.rotate_left(rotate_angle);
  }
}

void MainWindow::on_button_rotate_right()
{
  long double rotate_angle = atoi(entry_rotate_angle.get_text().raw().c_str());

  if (rotate_angle == 0)
  {
    entry_move_length.set_text(DEFAULT_ROTATE_ANGLE);
  }
  else
  {
    this->viewPort.rotate_right(rotate_angle);
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
    this->viewPort.removeObject(name);
  }
}

