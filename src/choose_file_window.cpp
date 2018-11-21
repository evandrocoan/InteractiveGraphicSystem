/*
 * choose_file_window.cpp
 *
 *  Created on: 23 de mar de 2018
 *      Author: karla
 */

#include "choose_file_window.h"

ChooseFileWindow::ChooseFileWindow(Gtk::FileChooserAction file_chooser_action)
: Gtk::FileChooserDialog("Choose a wavefront file", file_chooser_action),
  selected_path("")
{
  add_button("_Cancel", Gtk::RESPONSE_CANCEL);
  add_button("Select", Gtk::RESPONSE_OK);

  auto filter_any = Gtk::FileFilter::create();
  filter_any->set_name("Any files");
  filter_any->add_pattern("*");
  add_filter(filter_any);

  int result = run();

  switch(result)
  {
    case(Gtk::RESPONSE_OK):
    {
      this->selected_path = get_filename();
      //LOG(2, "The file : [ " + this->selected_path + " ] was selected.\n" );
      close();
      break;
    }
    case(Gtk::RESPONSE_CANCEL):
    {
    	//LOG(2, "Canceled the file select.\n");
      close();
      break;
    }
    default:
    {
      //LOG(2, "Unexpected button clicked.\n");
      close();
      break;
    }
  }
}

ChooseFileWindow::~ChooseFileWindow() {
	// TODO Auto-generated destructor stub
}

std::string ChooseFileWindow::get_file_path()
{
  return this->selected_path;
}

