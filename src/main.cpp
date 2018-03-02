#include "mainwindow.h"
#include "debugger.h"
#include <gtkmm/application.h>
#include <list>
#include <string>

int main (int argc, char *argv[])
{
  LOG(a1, "Starting the main application...");
  Glib::RefPtr<Gtk::Application> app = Gtk::Application::create(argc, argv);

  MainWindow mainwindow;

  LOG(a1, "Shows the main window and returns when it is closed.");
  return app->run(mainwindow);
}
