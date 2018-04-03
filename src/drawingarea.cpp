#include "drawingarea.h"

DrawingArea::DrawingArea(const World& _world, const ViewWindow& _viewWindow) :
      _world(_world),
      _viewWindow(_viewWindow)
{
  this->signal_size_allocate().connect(sigc::mem_fun(*this, &DrawingArea::on_my_size_allocate));
}

DrawingArea::~DrawingArea()
{
}

DrawingArea::UpdateViewPortSize::Connection DrawingArea::addObserver(const DrawingArea::UpdateViewPortSize::Callback& callback)
{
  auto connection = this->_updateViewPortSize.connect(callback);
  return connection;
}

/**
 * [DrawingArea::on_draw description]
 *
 * @param `cairo_context` Context is the main class used to draw in cairomm. It contains the current
 *     state of the rendering device, including coordinates of yet to be drawn shapes.
 *
 * @return               [description]
 */
bool DrawingArea::on_draw(const Cairo::RefPtr<Cairo::Context>& cairo_context)
{
  // LOG(8, "Chama-mes 5 vezes seguidas para desenhar a mesma coisa por que?");
  // LOG(8, "Paint white background");
  cairo_context->set_source_rgb(1, 1, 1);
  cairo_context->paint();

  this->_draw_clipping_axes(cairo_context);

  // LOG(8, "Set color's objects as black:");
  cairo_context->stroke();
  cairo_context->set_source_rgb(0, 0, 0);

  // LOG(8, "Draw displayFile objects");
  auto objects = this->_world.displayFile().getObjects();

  for (auto object : objects)
  {
    // auto coordinates = object->getWindowCoordinates();
    auto coordinates = object->getClippingCoordinates();
    int coordinates_count = coordinates.size();

    // LOG(8, "Skip objects which were completely clipped out of the Window");
    if (coordinates_count == 0)
    {
      // LOG(1, "ERROR: The object `%s` has no coordinates.", *object);
      continue;
    }

    auto firstCoordinate = this->_viewWindow.convertCoordinateToViewPort(**(coordinates.begin()));

    LOG(8, "object coordinates: %s", *object);
    cairo_context->move_to(firstCoordinate.x, firstCoordinate.y);

    if (coordinates_count == 1)
    {
      cairo_context->line_to(firstCoordinate.x+1, firstCoordinate.y+1);
    }
    else
    {
      for (auto coordinate : coordinates)
      {
        Coordinate coordinateConverted = this->_viewWindow.convertCoordinateToViewPort(*coordinate);
        cairo_context->line_to(coordinateConverted.x, coordinateConverted.y);
      }

      // LOG(8, "Draw a line until the first coordinate, closing the polygon")
      cairo_context->line_to(firstCoordinate.x, firstCoordinate.y);
    }
  }

  cairo_context->stroke();
  return true;
}

/**
 * Recalculate the clipping window size when the window size changes.
 *
 * @param `allocation` is the Current DrawingArea widget size like 556x469. Allocation is a
 *     structure holding the position and size of a rectangle. The intersection of two rectangles
 *     can be computed with intersect(). To find the union of two rectangles use join().
 *     Gtk::Allocation is a typedef of Gdk::Rectangle because GtkAllocation is a typedef of
 *     GdkRectangle.
 */
void DrawingArea::on_my_size_allocate(Gtk::Allocation& allocation)
{
  big_double width = allocation.get_width();
  big_double height = allocation.get_height();

  this->_updateViewPortSize(width, height);
}

void DrawingArea::_draw_clipping_axes(const Cairo::RefPtr<Cairo::Context>& cairo_context)
{
  // cairo_context->set_line_width(1);
  // cairo_context->set_source_rgb(0.741176, 0.717647, 0.419608);
  // Coordinate originOnWindow(0, 0);
  // Coordinate originOnWorld = this->_viewWindow.convertCoordinateToViewPort(originOnWindow);

  // LOG(8, "Drawing X and Y axes with originOnWorld: %s", originOnWorld);
  // LOG(4, "Drawing axes X from (%s, %s) to (%s, %s)", this->viewPort.xMin, originOnWorld.y, this->viewPort.xMax, originOnWorld.y );
  // LOG(4, "Drawing axes Y from (%s, %s) to (%s, %s)", originOnWorld.x, this->viewPort.yMin, originOnWorld.x, this->viewPort.yMax );
  // cairo_context->move_to(this->viewPort.xMin + CLIPPING_WINDOW_MARGIN_DISTANCE, originOnWorld.y);
  // cairo_context->line_to(this->viewPort.xMax - CLIPPING_WINDOW_MARGIN_DISTANCE, originOnWorld.y);
  // cairo_context->move_to(originOnWorld.x, this->viewPort.yMin + CLIPPING_WINDOW_MARGIN_DISTANCE);
  // cairo_context->line_to(originOnWorld.x, this->viewPort.yMax - CLIPPING_WINDOW_MARGIN_DISTANCE);
  // cairo_context->stroke();
  // LOG(8, "Draw the clipping window with a red border")
  cairo_context->set_source_rgb(0.99, 0.0, 0.0);

  cairo_context->line_to(this->_viewWindow.viewPort(0).x, this->_viewWindow.viewPort(0).y);
  cairo_context->line_to(this->_viewWindow.viewPort(1).x, this->_viewWindow.viewPort(1).y);
  cairo_context->line_to(this->_viewWindow.viewPort(2).x, this->_viewWindow.viewPort(2).y);
  cairo_context->line_to(this->_viewWindow.viewPort(3).x, this->_viewWindow.viewPort(3).y);
  cairo_context->line_to(this->_viewWindow.viewPort(0).x, this->_viewWindow.viewPort(0).y);
}
