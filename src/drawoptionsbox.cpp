#include "drawoptionsbox.h"
#include <iostream>

DrawOptionsBox::DrawOptionsBox(const Glib::ustring& title,
                               gint spacing,
                               Gtk::ButtonBoxStyle layout,
                               MainWindow* mainWindow
                              )
    : Gtk::Frame(title),
      mainWindow(mainWindow),
      button_add_object("Add Object"),
      button_move_up("up"),
      button_move_down("down"),
      button_move_left("left"),
      button_move_right("right"),
      button_zoom_in("+"),
      button_zoom_out("-"),
      entry_move_length(),
      entry_zoom_scale(),
      objects_list(),
      button_delete_obj("Delete Object")
{
  LOG(2, "Entering...");

  LOG(4, "Determinando ações quando clicado cada botão;");
  button_move_up.signal_clicked().connect(sigc::mem_fun(*this, &DrawOptionsBox::on_button_move_up));
  button_move_down.signal_clicked().connect(sigc::mem_fun(*this, &DrawOptionsBox::on_button_move_down));
  button_move_left.signal_clicked().connect(sigc::mem_fun(*this, &DrawOptionsBox::on_button_move_left));
  button_move_right.signal_clicked().connect(sigc::mem_fun(*this, &DrawOptionsBox::on_button_move_right));

  button_zoom_in.signal_clicked().connect(sigc::mem_fun(*this, &DrawOptionsBox::on_button_zoom_in));
  button_zoom_out.signal_clicked().connect(sigc::mem_fun(*this, &DrawOptionsBox::on_button_zoom_out));

  button_add_object.signal_clicked().connect(sigc::mem_fun(*this, &DrawOptionsBox::on_button_add_object));
  button_delete_obj.signal_clicked().connect(sigc::mem_fun(*this, &DrawOptionsBox::on_button_delete_object));

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

  Gtk::ButtonBox* bbox = Gtk::manage( new Gtk::ButtonBox(Gtk::ORIENTATION_VERTICAL) );

  bbox->set_border_width(10);
  add(*bbox);

  bbox->set_layout(layout);
  bbox->set_spacing(spacing);
  bbox->add(grid_list_obj);
  bbox->add(grid_move);
  bbox->add(grid_zoom);
}

DrawOptionsBox::~DrawOptionsBox()
{
}

void DrawOptionsBox::on_button_move_up()
{
  int move_length = atoi(entry_move_length.get_text().raw().c_str());

  if (move_length == 0)
  {
    entry_move_length.set_text(DEFAULT_MOVE_LENGTH);
  }
  else
  {
    this->mainWindow->getViewport()->getViewWindow()->move_up(move_length);
    this->mainWindow->getViewport()->queue_draw();
  }
}

void DrawOptionsBox::on_button_move_down()
{
  int move_length = atoi(entry_move_length.get_text().raw().c_str());

  if (move_length == 0)
  {
    entry_move_length.set_text(DEFAULT_MOVE_LENGTH);
  }
  else
  {
    this->mainWindow->getViewport()->getViewWindow()->move_down(move_length);
    this->mainWindow->getViewport()->queue_draw();
  }
}

void DrawOptionsBox::on_button_move_left()
{
  int move_length = atoi(entry_move_length.get_text().raw().c_str());

  if (move_length == 0)
  {
    entry_move_length.set_text(DEFAULT_MOVE_LENGTH);
  }
  else
  {
    this->mainWindow->getViewport()->getViewWindow()->move_left(move_length);
    this->mainWindow->getViewport()->queue_draw();
  }
}

void DrawOptionsBox::on_button_move_right()
{
  int move_length = atoi(entry_move_length.get_text().raw().c_str());

  if (move_length == 0)
  {
    entry_move_length.set_text(DEFAULT_MOVE_LENGTH);
  }
  else
  {
    this->mainWindow->getViewport()->getViewWindow()->move_right(move_length);
    this->mainWindow->getViewport()->queue_draw();
  }
}

void DrawOptionsBox::on_button_zoom_in()
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
    this->mainWindow->getViewport()->getViewWindow()->zoom_in(zoom_scale);
    this->mainWindow->getViewport()->queue_draw();
  }
}

void DrawOptionsBox::on_button_zoom_out()
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
    this->mainWindow->getViewport()->getViewWindow()->zoom_out(zoom_scale);
    this->mainWindow->getViewport()->queue_draw();
  }
}

void DrawOptionsBox::update_list_object()
{
  LOG(2, "Entering...");
  LOG(4, "limpa a lista de objetos para reimprimi-la");
  this->objects_list.remove_all();
  std::list<std::string> names = this->mainWindow->getViewport()->getViewWindow()->getDisplayFile()->getNamesList();

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

void DrawOptionsBox::on_button_add_object()
{
  LOG(2, "Entering...");
  add_object_window = new AddObjectWindow(this->mainWindow, this);
  add_object_window->show();

  //add_object_window->on_button_save_point();
  this->update_list_object();
}

void DrawOptionsBox::on_button_delete_object()
{
  Glib::ustring name = objects_list.get_active_text();

  if(!(name.empty()))
  {
    this->mainWindow->getViewport()->getViewWindow()->getDisplayFile()->removeObjectByName((string) name);
    this->mainWindow->getViewport()->queue_draw();

    this->update_list_object();
  }
}
