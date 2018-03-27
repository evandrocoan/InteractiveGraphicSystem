/*
 * ChooseFileWindow.h
 *
 *  Created on: 23 de mar de 2018
 *      Author: karla
 */

#ifndef CHOOSEFILEWINDOW_H_
#define CHOOSEFILEWINDOW_H_

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

#endif /* CHOOSEFILEWINDOW_H_ */
