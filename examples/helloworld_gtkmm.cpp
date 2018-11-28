

/**
 * To build it run:
 *     g++ helloworld_gtkmm.cpp -o helloworld_gtkmm `pkg-config gtkmm-3.0 --cflags --libs`
 *
 * Simple Example - Programming with gtkmm 3 / Basics
 * https://developer.gnome.org/gtkmm-tutorial/stable/sec-basics-simple-example.html.en
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
