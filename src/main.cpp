#include "mainwindow.h"
#include "debugger.h"
#include <gtkmm/application.h>
#include <list>
#include <string>

int main (int argc, char *argv[])
{
  LOG(1, "Starting the main application...");

  Glib::RefPtr<Gtk::Application> app = Gtk::Application::create(argc, argv);
  MainWindow mainwindow;

  LOG(1, "Shows the main window and returns when it is closed.");
  int exit_code = app->run(mainwindow);

  LOG(1, "exit_code: %d", exit_code);
  return exit_code;
}
