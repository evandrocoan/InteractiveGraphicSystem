#include <gtkmm/application.h>
#include <list>
#include <string>

#include "mainwindow.h"
#include "debugger.h"

/**
 * You can enable or disable the debugging statements from `LOG()` by going to the file
 * `src/debugger.h` and looking for the variable `DEBUG_LEVEL`.
 *
 * You can control the debug level used by the function `LOG()` by going to the file
 * `src/debugger.h` and looking for the variable `_debugger_int_debug_level`.
 */
int main (int argc, char* argv[])
{
  LOG(2, "Starting the main application...");

  // C++ catching all exceptions
  // https://stackoverflow.com/questions/315948/c-catching-all-exceptions
  try
  {
    Glib::RefPtr<Gtk::Application> app = Gtk::Application::create(argc, argv);
    MainWindow mainWindow;

    LOG(4, "Shows the main window and returns when it is closed.");
    int exit_code = app->run(mainWindow.getWindow());

    LOG(2, "exit_code: %d", exit_code);
    return exit_code;
  }
  catch( const std::exception &e)
  {
    LOG(1, "");
    LOG(1, "");
    LOG(1, "A uncaught `std::exception` was throw with the message: %s", e.what());
  }

  return 1;
}
