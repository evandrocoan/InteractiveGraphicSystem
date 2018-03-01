#ifndef GTKMM_APP_DRAW_OPTIONS_BOX
#define GTKMM_APP_DRAW_OPTIONS_BOX

/* Include any library as need to use other's components such as
 * gtkmm/button or gtkmm/frame.
 */
#include <gtkmm/frame.h>
#include <gtkmm/button.h>
#include <gtkmm/buttonbox.h>
#include <gtkmm/comboboxtext.h>
#include <gtkmm/grid.h>
#include "mainwindow.h"
#include "viewwindow.h"
#include "addobjectwindow.h"

#define DEFAULT_MOVE_LENGTH "10"
#define DEFAULT_ZOOM_SCALE 1.5

class DrawOptionsBox: public Gtk::Frame {

public:
	DrawOptionsBox(const Glib::ustring& title, gint spacing, Gtk::ButtonBoxStyle layout, MainWindow* mainWindow);
	virtual ~DrawOptionsBox();

protected:
	MainWindow* mainWindow;
	Gtk::Button button_add_object, button_move_up, button_move_down,
				button_move_left, button_move_right, button_zoom_in,
				button_zoom_out,button_delete_obj;
	Gtk::Grid grid_move, grid_zoom, grid_list_obj;
	AddObjectWindow* add_object_window;
	Gtk::Entry entry_move_length, entry_zoom_scale;
	Gtk::ComboBoxText objects_list;

	void on_button_add_object();
	void on_button_move_up();
	void on_button_move_down();
	void on_button_move_left();
	void on_button_move_right();
	void on_button_zoom_in();
	void on_button_zoom_out();
	void update_list_object();
	void on_button_delete_object();
};

class draw_options_box {
public:
	draw_options_box();
	virtual ~draw_options_box();
};
#endif // GTKMM_APP_DRAW_OPTIONS_BOX

