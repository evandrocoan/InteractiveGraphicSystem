/*
 * choose_file_window.h
 *
 *  Created on: 23 de mar de 2018
 *      Author: karla
 */

#ifndef CHOOSE_FILE_WINDOW_H_
#define CHOOSE_FILE_WINDOW_H_

#include <gtkmm/buttonbox.h>
#include <gtkmm/filechooserdialog.h>
#include <gtkmm/button.h>
#include <gtkmm/filefilter.h>
#include <string>

class ChooseFileWindow : public Gtk::FileChooserDialog
{
public:
  ChooseFileWindow(Gtk::FileChooserAction file_chooser_action);
  virtual ~ChooseFileWindow();
  std::string get_file_path();

protected:
  std::string selected_path;
};

#endif // CHOOSE_FILE_WINDOW_H_
