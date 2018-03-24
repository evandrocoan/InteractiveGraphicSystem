#include "mainwindow.h"

/**
 * https://en.wikipedia.org/wiki/Template:Unicode_chart_Arrows
 */
MainWindow::MainWindow() :
      addObject(this->drawingArea),
      addTransformation(this->drawingArea),
      button_move_up("↑"),
      button_move_down("↓"),
      button_move_left("←"),
      button_move_right("→"),
      button_zoom_in("+"),
      button_zoom_out("-"),
      button_add_object("Add"),
      button_delete_object("Rem"),
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
  this->main_box.pack_start(this->right_frame, Gtk::PACK_EXPAND_WIDGET, 10);
  this->right_frame.add(this->drawingArea);
  this->drawingArea.show();
  this->drawingArea.addObserver(std::bind(&MainWindow::updateDropdownList, this));

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

void MainWindow::setDefaultTooltips()
{
  button_add_object   .set_tooltip_text("Add new object");
  button_delete_object.set_tooltip_text("Remove current selected object");
  objects_list        .set_tooltip_text("The list of all created objects on the DrawingArea");

  entry_move_length   .set_tooltip_text("How many pixels to move the ViewWindow");
  entry_zoom_scale    .set_tooltip_text("A scaling factor as `1.1` or `0.9` to `zoom in` or `zoom out` the ViewWindow");
  button_zoom_in      .set_tooltip_text("Apply the scaling factor to the ViewWindow over the Drawing World");
  button_zoom_out     .set_tooltip_text("Apply inverted the scaling factor to the ViewWindow over the Drawing World");

  button_move_up      .set_tooltip_text("Move the ViewWindow on the drawing area upwards");
  button_move_down    .set_tooltip_text("Move the ViewWindow on the drawing area downwards");
  button_move_left    .set_tooltip_text("Move the ViewWindow on the drawing area to the left");
  button_move_right   .set_tooltip_text("Move the ViewWindow on the drawing area to the right");
}

void MainWindow::setupButtons()
{
  LOG(4, "Inicializando dado da entrada do tamanho de movimentação");
  entry_move_length.set_width_chars(3);
  entry_move_length.set_text(default_move_length);

  LOG(4, "Inicializando dado da entrada do tamanho do zoom");
  entry_zoom_scale.set_width_chars(3);
  char array[4];

  sprintf(array, "%f", default_zoom_scale);
  array[3] = '\0';
  entry_zoom_scale.set_text(array);

  LOG(4, "Montando a estrutura da grade de lista de objetos");
  grid_list_obj.set_column_homogeneous(true);
  grid_list_obj.attach(button_add_object,    1, 1, 1, 1);
  grid_list_obj.attach(button_delete_object, 2, 1, 1, 1);
  grid_list_obj.attach(objects_list,         1, 2, 2, 1);

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

  LOG(4, "Adding the draw options box to left frame");
  left_box.set_border_width(10);
  left_box.set_spacing(10);
  left_box.add(grid_list_obj);
  left_box.add(grid_move);
  left_box.add(grid_zoom);
  left_box.add(this->addTransformation.getBox());
}

void MainWindow::connectButtons()
{
  LOG(4, "Determinando ações quando clicado cada botão;");
  this->button_move_up.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_button_move_up));
  this->button_move_down.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_button_move_down));
  this->button_move_left.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_button_move_left));
  this->button_move_right.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_button_move_right));

  this->button_zoom_in.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_button_zoom_in));
  this->button_zoom_out.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_button_zoom_out));

  this->button_add_object.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_button_add_object));
  this->button_delete_object.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_button_delete_object));
}

/**
 * Called when the `DrawingArea` objects list is updated.
 */
void MainWindow::updateDropdownList()
{
  LOG(2, "Entering...");
  auto names = this->drawingArea.getNamesList();

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
    entry_move_length.set_text(default_move_length);
  }
  else
  {
    this->drawingArea.move_up(move_length);
  }
}

void MainWindow::on_button_move_down()
{
  int move_length = atoi(entry_move_length.get_text().raw().c_str());

  if (move_length == 0)
  {
    entry_move_length.set_text(default_move_length);
  }
  else
  {
    this->drawingArea.move_down(move_length);
  }
}

void MainWindow::on_button_move_left()
{
  int move_length = atoi(entry_move_length.get_text().raw().c_str());

  if (move_length == 0)
  {
    entry_move_length.set_text(default_move_length);
  }
  else
  {
    this->drawingArea.move_left(move_length);
  }
}

void MainWindow::on_button_move_right()
{
  int move_length = atoi(entry_move_length.get_text().raw().c_str());

  if (move_length == 0)
  {
    entry_move_length.set_text(default_move_length);
  }
  else
  {
    this->drawingArea.move_right(move_length);
  }
}

void MainWindow::on_button_zoom_in()
{
  float zoom_scale = atof(entry_zoom_scale.get_text().raw().c_str());

  if (zoom_scale <=1)
  {
    char array[4];
    sprintf(array, "%f", default_zoom_scale);

    array[3] = '\0';
    entry_zoom_scale.set_text(array);
  }
  else
  {
    this->drawingArea.zoom_in(zoom_scale);
  }
}

void MainWindow::on_button_zoom_out()
{
  float zoom_scale = atof(entry_zoom_scale.get_text().raw().c_str());

  if (zoom_scale <=1)
  {
    char array[4];
    sprintf(array, "%f", default_zoom_scale);

    array[3] = '\0';
    entry_zoom_scale.set_text(array);
  }
  else
  {
    this->drawingArea.zoom_out(zoom_scale);
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
    this->drawingArea.removeObject(name);
  }
}

