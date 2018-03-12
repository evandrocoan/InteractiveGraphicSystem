#include "mainwindow.h"

MainWindow::MainWindow() :
      main_box(Gtk::ORIENTATION_HORIZONTAL),
      left_box(Gtk::ORIENTATION_VERTICAL),
      right_frame("ViewPort"),
      button_add_object("Add Object"),
      button_move_up("up"),
      button_move_down("down"),
      button_move_left("left"),
      button_move_right("right"),
      button_zoom_in("+"),
      button_zoom_out("-"),
      button_delete_obj("Delete Object"),
      addObjectWindow(&this->viewport)
{
  LOG(2, "Entering...");
  this->set_title("CG - Trabalho01 - Karla Ap. Justen, Evandro S. Coan");
  this->set_border_width(15);
  this->set_default_size(700, 500);

  LOG(4, "Draw options");
  this->main_box.pack_start(left_frame, Gtk::PACK_SHRINK, 10);
  this->left_box.set_border_width(10);
  this->left_frame.add(left_box);
  this->setupButtons("", 10, Gtk::BUTTONBOX_START);
  this->connectButtons();

  LOG(4, "ViewPort");
  this->main_box.pack_start(this->right_frame, Gtk::PACK_EXPAND_WIDGET, 10);
  this->right_frame.add(this->viewport);
  this->viewport.show();

  LOG(4, "Show all components");
  this->add(this->main_box);
  this->show_all_children();
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupButtons(const Glib::ustring& title, gint spacing, Gtk::ButtonBoxStyle layout)
{
  LOG(4, "Inicializando dado da entrada do tamanho de movimentação");
  entry_move_length.set_width_chars(1);
  entry_move_length.set_text(DEFAULT_MOVE_LENGTH);

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
  grid_list_obj.attach(button_delete_obj, 1, 3, 1, 1);

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

  LOG(4, "Adding the draw options box to left frame");
  auto buttons_frame = new Gtk::Frame(title);
  Gtk::ButtonBox* buttonBox = Gtk::manage(new Gtk::ButtonBox(Gtk::ORIENTATION_VERTICAL));

  buttonBox->set_border_width(10);
  buttonBox->set_layout(layout);
  buttonBox->set_spacing(spacing);
  buttonBox->add(grid_list_obj);
  buttonBox->add(grid_move);
  buttonBox->add(grid_zoom);

  buttons_frame->add(*buttonBox);
  left_box.pack_start(*Gtk::manage(buttons_frame), Gtk::PACK_EXPAND_WIDGET);

  this->viewport.addObserver(this);
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
  this->button_delete_obj.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_button_delete_object));
}

/**
 * Called when the `ViewPort` objects list is updated.
 */
void MainWindow::updateDropdownList()
{
  LOG(2, "Entering...");
  auto names = this->viewport.getDisplayFile()->getNamesList();

  LOG(4, "limpa a lista de objetos para reimprimi-la");
  this->objects_list.remove_all();

  /*for(std::list<std::string>::iterator it = names.begin(); it != names.end(); it++)
  {
    this->objects_list.append(*it);
  }*/

  //for(std::string object : names)
  for(auto object : names)
  {
    this->objects_list.append(object);
    LOG(4, object.c_str());
  }
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
    this->viewport.getViewwindow()->move_up(move_length);
    this->viewport.queue_draw();
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
    this->viewport.getViewwindow()->move_down(move_length);
    this->viewport.queue_draw();
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
    this->viewport.getViewwindow()->move_left(move_length);
    this->viewport.queue_draw();
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
    this->viewport.getViewwindow()->move_right(move_length);
    this->viewport.queue_draw();
  }
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
    this->viewport.getViewwindow()->zoom_in(zoom_scale);
    this->viewport.queue_draw();
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
    this->viewport.getViewwindow()->zoom_out(zoom_scale);
    this->viewport.queue_draw();
  }
}

void MainWindow::on_button_add_object()
{
  LOG(2, "Entering...");
  this->addObjectWindow.show();
  //addObjectWindow->on_button_save_point();
}

void MainWindow::on_button_delete_object()
{
  Glib::ustring name = objects_list.get_active_text();

  if(!(name.empty()))
  {
    this->viewport.removeObject((string)name);
  }
}

