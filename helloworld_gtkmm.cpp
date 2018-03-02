

/**
 * To build it run:
 *     g++ helloworld2.cpp -o helloworld2 `pkg-config gtkmm-3.0 --cflags --libs`
 *
 * To install GTKmm on windows, you will need to use MSYS2. See:
 * https://wiki.gnome.org/Projects/gtkmm/MSWindows
 *
 */

#include <gtkmm.h>

int main(int argc, char *argv[])
{
  auto app =
    Gtk::Application::create(argc, argv,
      "org.gtkmm.examples.base");

  Gtk::Window window;
  window.set_default_size(200, 200);

  return app->run(window);
}
