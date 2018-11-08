#ifndef GTKMM_APP_DRAWINGAREA
#define GTKMM_APP_DRAWINGAREA

#include <list>
#include <string>
#include <iomanip>

#include <gtkmm/drawingarea.h>
#include <gdkmm/rgba.h>
#include <cairomm/context.h>

#include "traits.h"
#include "debugger.h"
#include "subject_controller.h"

#include "point.h"
#include "line.h"
#include "polygon.h"
#include "coordinate.h"

#include "world.h"
#include "viewwindow.h"
#include "displayfile.h"

/**
 * The Drawing Area Widget
 * https://developer.gnome.org/gtkmm-tutorial/stable/chapter-drawingarea.html.en
 *
 * Drawing Straight Lines
 * https://developer.gnome.org/gtkmm-tutorial/stable/sec-cairo-drawing-lines.html.en#cairo-example-lines
 */
class DrawingArea : public Gtk::DrawingArea, public NonCopyable
{
public:
  DrawingArea(const World&, const ViewWindow&);
  virtual ~DrawingArea();

  friend std::ostream& operator<<(std::ostream &output, const DrawingArea &object);

  /**
   * Implementations types for the Observer Design Pattern with C++ 11 templates and function
   * pointers, instead of tight coupled inheritance.
   */
  typedef Signal<unsigned int, unsigned int> UpdateViewPortSize;
  UpdateViewPortSize::Connection addObserver(const UpdateViewPortSize::Callback&);

protected:
  UpdateViewPortSize _updateViewPortSize;

  const World& _world;
  const ViewWindow& _viewWindow;

  void on_my_size_allocate(Gtk::Allocation&);

  bool on_draw(const Cairo::RefPtr<Cairo::Context>&) override;
  void _draw_clipping_axes(const Cairo::RefPtr<Cairo::Context>&);

private:
  bool drawn_circle(const Cairo::RefPtr<Cairo::Context>&, const DrawableObject*);
  bool drawn_general(const Cairo::RefPtr<Cairo::Context>&, const DrawableObject*);

  void drawn_point(const Cairo::RefPtr<Cairo::Context>&, const DrawableObject*);
  void drawn_polygon(const Cairo::RefPtr<Cairo::Context>&, const DrawableObject*);
};

#endif // GTKMM_APP_DRAWINGAREA
