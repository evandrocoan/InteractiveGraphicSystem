#include "mainwindow.h"
#include "drawoptionsbox.h"

MainWindow::MainWindow()
    : main_box(Gtk::ORIENTATION_HORIZONTAL),
      left_box(Gtk::ORIENTATION_VERTICAL),
      left_frame(),
      right_frame("Viewport"),
      viewport()
{
  LOG(1, "Entering...");
  set_border_width(15);
  set_title("CG - Trabalho01 - Karla Ap. Justen, Evandro S. Coan");

  set_default_size(700, 500);
  add(main_box);

  LOG(1, "Draw options");
  main_box.pack_start(left_frame, Gtk::PACK_SHRINK, 10);
  left_box.set_border_width(10);
  left_frame.add(left_box);

  LOG(1, "Adding the draw options box to left frame");
  auto draw_options_box = new DrawOptionsBox("", 10, Gtk::BUTTONBOX_START, this);
  left_box.pack_start(*Gtk::manage(draw_options_box), Gtk::PACK_EXPAND_WIDGET);

  LOG(1, "Viewport");
  main_box.pack_start(right_frame, Gtk::PACK_EXPAND_WIDGET, 10);
  right_frame.add(viewport);
  viewport.show();

  LOG(1, "Show all components");
  show_all_children();
}

Viewport* MainWindow::getViewport()
{
  return &this->viewport;
}

MainWindow::~MainWindow()
{
}
