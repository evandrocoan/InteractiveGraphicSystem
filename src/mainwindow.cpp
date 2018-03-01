#include "mainwindow.h"
#include "drawoptionsbox.h"

MainWindow::MainWindow()
    : main_box(Gtk::ORIENTATION_HORIZONTAL),
      left_box(Gtk::ORIENTATION_VERTICAL),
      left_frame(),
      right_frame("Viewport"),
      viewport()
{

	set_border_width(15);
	set_title("CG - Trabalho01 - Karla Ap. Justen");
	set_default_size(700, 500);

	add(main_box);

	// draw options
	main_box.pack_start(left_frame, Gtk::PACK_SHRINK, 10);
	left_box.set_border_width(10);
	left_frame.add(left_box);
	// Adding the draw options box to left frame
	left_box.pack_start(
				*Gtk::manage(new DrawOptionsBox("", 10, Gtk::BUTTONBOX_START, this)), Gtk::PACK_EXPAND_WIDGET );


	// viewport
	main_box.pack_start(right_frame, Gtk::PACK_EXPAND_WIDGET, 10);
	right_frame.add(viewport);
	viewport.show();
	// show all components
	show_all_children();
}

Viewport* MainWindow::getViewport()
{
  return &this->viewport;
}

MainWindow::~MainWindow()
{
}
