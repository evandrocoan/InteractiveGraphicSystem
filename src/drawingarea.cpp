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

  // LOG(8, "Draw displayFile objects");
  auto objects = this->_world.displayFile().getObjects();

  for (auto object : objects)
  {
    LOG(8, "object: %s", *object);

    if( !object->isDrawable() )
    {
      LOG(8, "Skip objects which were completely clipped out of the Window");
      continue;
    }

    // https://stackoverflow.com/questions/351845/finding-the-type-of-an-object-in-c
    if( object->curve_type )
    {
      if( object->curve_type == CurveType::BEZIER || object->curve_type == CurveType::BSPLINE )
      {
        if( drawn_circle( cairo_context, object ) ) continue;
      }
      else
      {
        std::string error = tfm::format( "Invalid curve type object: %s", object );
        throw std::runtime_error( error );
      }
    }
    else
    {
      if( drawn_general( cairo_context, object ) ) continue;
    }
  }

  return true;
}

bool DrawingArea::drawn_general(const Cairo::RefPtr<Cairo::Context>& cairo_context, const DrawableObject* object)
{
  // auto coordinates = object->windowCoordinates();
  auto coordinates = object->clippingCoordinates();
  auto coordinates_count = coordinates.size();

  if( coordinates_count == 0 )
  {
    LOG(1, "ERROR: The object `%s` has no coordinates.", *object);
    return true;
  }

  if( coordinates_count == 1 )
  {
    drawn_point(cairo_context, object);
  }
  else
  {
    drawn_polygon(cairo_context, object);
  }

  return false;
}

void DrawingArea::drawn_point(const Cairo::RefPtr<Cairo::Context>& cairo_context, const DrawableObject* object)
{
  auto coordinates = object->clippingCoordinates();
  auto firstCoordinate = this->_viewWindow.convertCoordinateToViewPort(**(coordinates.begin()));

  cairo_context->move_to(firstCoordinate->x, firstCoordinate->y);
  cairo_context->line_to(firstCoordinate->x+1, firstCoordinate->y+1);
  cairo_context->stroke(); // outline it
}

void DrawingArea::drawn_polygon(const Cairo::RefPtr<Cairo::Context>& cairo_context, const DrawableObject* object)
{
  Coordinate* coordinateConverted;
  auto coordinates = object->clippingCoordinates();

  auto border = object->borderColor();
  auto filling = object->fillingColor();

  // LOG(8, "border: %s", border);
  // LOG(8, "filing: %s", filling);
  cairo_context->set_source_rgb(border.x, border.y, border.z);

  for( auto coordinate : coordinates )
  {
    coordinateConverted = this->_viewWindow.convertCoordinateToViewPort(*coordinate);
    cairo_context->line_to(coordinateConverted->x, coordinateConverted->y);
  }

  // https://developer.gnome.org/gtkmm-tutorial/stable/sec-cairo-drawing-arcs.html.en
  // LOG(8, "Line back to start point, closing the polygon")
  cairo_context->save();
  cairo_context->close_path();
  cairo_context->set_source_rgb(filling.x, filling.y, filling.z);
  cairo_context->fill_preserve();
  cairo_context->restore();  // back to opaque black
  cairo_context->stroke();  // outline it
}

bool DrawingArea::drawn_circle(const Cairo::RefPtr<Cairo::Context>& cairo_context, const DrawableObject* object)
{
  auto lines = object->lines;
  Coordinate* coordinateConverted;

  if( lines.size() == 0 )
  {
    LOG(1, "ERROR: The object `%s` has no coordinates.", *object);
    return true;
  }

  for( auto line : object->lines )
  {
    // auto coordinates = line->windowCoordinates();
    auto coordinates = line->clippingCoordinates();

    coordinateConverted = this->_viewWindow.convertCoordinateToViewPort(*coordinates[0]);
    LOG( 1, "coordinateConverted1: %s, from: %s", *coordinateConverted, *coordinates[0] );
    cairo_context->move_to(coordinateConverted->x, coordinateConverted->y);

    coordinateConverted = this->_viewWindow.convertCoordinateToViewPort(*coordinates[1]);
    LOG( 1, "coordinateConverted2: %s, from: %s", *coordinateConverted, *coordinates[1] );
    cairo_context->move_to(coordinateConverted->x, coordinateConverted->y);
  }

  cairo_context->stroke();  // outline it
  return false;
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
  // Coordinate* originOnWorld = this->_viewWindow.convertCoordinateToViewPort(originOnWindow);

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

  cairo_context->line_to(this->_viewWindow.viewPort(0)->x, this->_viewWindow.viewPort(0)->y);
  cairo_context->line_to(this->_viewWindow.viewPort(1)->x, this->_viewWindow.viewPort(1)->y);
  cairo_context->line_to(this->_viewWindow.viewPort(2)->x, this->_viewWindow.viewPort(2)->y);
  cairo_context->line_to(this->_viewWindow.viewPort(3)->x, this->_viewWindow.viewPort(3)->y);
  cairo_context->line_to(this->_viewWindow.viewPort(0)->x, this->_viewWindow.viewPort(0)->y);
  cairo_context->stroke();
}
