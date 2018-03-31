#include "drawingarea.h"

DrawingArea::DrawingArea() :
      viewPort(),
      viewWindow(),
      isCentered(false),
      displayFile()
{
  this->signal_size_allocate().connect(sigc::mem_fun(*this, &DrawingArea::on_my_size_allocate));
  this->_connection = this->viewWindow.addObserver(std::bind(&DrawingArea::updateObjectCoordinates, this));
}

DrawingArea::~DrawingArea()
{
  this->disconnectObserver();
}

void DrawingArea::disconnectObserver()
{
  if( !this->_connection.disconnect() )
  {
    LOG(1, "");
    LOG(1, "");
    LOG(1, "ERROR! Could not disconnect the object `%s` from its observer.", *this);
  }
}

/**
 * Prints a beauty version of the viewWindow when called on `std::cout<< viewWindow << std::end;`
 */
std::ostream& operator<<( std::ostream &output, const DrawingArea &object )
{
  output
      << object.viewPort << " "
      << object.viewWindow;
  return output;
}

/**
 * Move the X and Y axes to the center of the window when the program starts and recalculate the
 * clipping window size when the window size changes.
 *
 * @param `allocation` is the Current DrawingArea widget size like 556x469. Allocation is a
 *     structure holding the position and size of a rectangle. The intersection of two rectangles
 *     can be computed with intersect(). To find the union of two rectangles use join().
 *     Gtk::Allocation is a typedef of Gdk::Rectangle because GtkAllocation is a typedef of
 *     GdkRectangle.
 */
void DrawingArea::on_my_size_allocate(Gtk::Allocation& allocation)
{
  big_double width = this->get_width();
  big_double height = this->get_height();

  // TODO on `updateViewPortSize()`: Keep viewPort and viewWindow portions while resizing the viewPort
  this->updateViewPortSize(width, height);

  if( !isCentered )
  {
    this->isCentered = true;
    this->draw_xy_axes();

    // LOG(4, "Moving ViewWindow (0, 0) to the window center...");
    // this->move(-this->viewWindow.width(), -this->viewWindow.height());
  }

  this->viewPort.updateClippingCoordinatesWindowSize(width, height);
  LOG(4, "Current viewport size %sx%s", width, height);
}

void DrawingArea::apply(std::string object_name, Transformation &transformation)
{
  if( this->displayFile.isObjectOnByName(object_name) )
  {
    if( transformation.size() )
    {
      this->displayFile.apply(object_name, transformation);
      this->viewWindow.callObservers();
    }
    else
    {
      LOG(4, "There are no transformations available to be applied on your object: `%s` %s", object_name, transformation);
    }
  }
  else
  {
    LOG(4, "No object was found within the name: `%s`", object_name);
  }
}

void DrawingArea::draw_xy_axes()
{
  LOG(4, "centerWindow: %d , %d",this->viewPort.xMax/2, this->viewPort.yMax/2);

  LOG(4, "Building the Y axe");
  Coordinate* bottom_axe = new Coordinate(0, -world_axes_size);
  Coordinate* top_axe = new Coordinate(0, world_axes_size);

  LOG(4, "Building the X axe");
  Coordinate* left_axe = new Coordinate(-world_axes_size, 0);
  Coordinate* right_axe = new Coordinate(world_axes_size, 0);

  Line* lineY = new Line("Y axe", bottom_axe, top_axe);
  Line* lineX = new Line("X axe", left_axe, right_axe);

  this->addObject(lineY);
  this->addObject(lineX);
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

  // cairo_context->set_line_width(1);
  // cairo_context->set_source_rgb(0.741176, 0.717647, 0.419608);
  // Coordinate originOnWindow(0, 0);
  // Coordinate originOnWorld = convertCoordinateToViewPort(originOnWindow);

  // LOG(8, "Drawing X and Y axes with originOnWorld: %s", originOnWorld);
  // LOG(4, "Drawing axes X from (%s, %s) to (%s, %s)", this->viewPort.xMin, originOnWorld.y, this->viewPort.xMax, originOnWorld.y );
  // LOG(4, "Drawing axes Y from (%s, %s) to (%s, %s)", originOnWorld.x, this->viewPort.yMin, originOnWorld.x, this->viewPort.yMax );
  // cairo_context->move_to(this->viewPort.xMin + clipping_window_margin_distance, originOnWorld.y);
  // cairo_context->line_to(this->viewPort.xMax - clipping_window_margin_distance, originOnWorld.y);
  // cairo_context->move_to(originOnWorld.x, this->viewPort.yMin + clipping_window_margin_distance);
  // cairo_context->line_to(originOnWorld.x, this->viewPort.yMax - clipping_window_margin_distance);
  // cairo_context->stroke();

  // LOG(8, "Draw the clipping window with a red border")
  cairo_context->set_source_rgb(0.99, 0.0, 0.0);

  cairo_context->line_to(this->viewPort.getPoint(0)->x, this->viewPort.getPoint(0)->y);
  cairo_context->line_to(this->viewPort.getPoint(1)->x, this->viewPort.getPoint(1)->y);
  cairo_context->line_to(this->viewPort.getPoint(2)->x, this->viewPort.getPoint(2)->y);
  cairo_context->line_to(this->viewPort.getPoint(3)->x, this->viewPort.getPoint(3)->y);
  cairo_context->line_to(this->viewPort.getPoint(0)->x, this->viewPort.getPoint(0)->y);

  // LOG(8, "Set color's objects as black:");
  cairo_context->stroke();
  cairo_context->set_source_rgb(0, 0, 0);

  // LOG(8, "Draw displayFile objects");
  auto objects = this->displayFile.getObjects();

  for (auto object : objects)
  {
    // auto coordinates = object->getViewWindowCoordinates();
    auto coordinates = object->getViewWindowCoordinates();
    int coordinates_count = coordinates.size();

    if (coordinates_count == 0)
    {
      LOG(1, "ERROR: The object `%s` has no coordinates.", *object);
      continue;
    }

    Coordinate firstCoordinate = this->convertCoordinateToViewPort(**(coordinates.begin()));

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
        Coordinate coordinateConverted = this->convertCoordinateToViewPort(*coordinate);
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
 * Transformada de viewport - Slide 2 de "02 - Conceitos Básicos"
 *
 * Resize `viewwindow` when `viewport` is resized:
 * http://www.di.ubi.pt/~agomes/cg/teoricas/04e-windows.pdf
 *
 * A strategy of keeping proportions automatically between window and viewport.
 *
 * Window-Viewport Mapping, important conclusion: As the world window increases in size the image in
 * viewport decreases in size and vice-versa.
 *
 * The user may enlarge or reduce the size of a viewport with w pixels wide and h pixels high by
 * pulling away the right-bottom of its interface window.
 *
 * To avoid distortion, we must change the size of the world window accordingly.
 *
 * For that, we assume that the initial world window is a square with side length L.
 *
 * A possible solution is to change the world window whenever the viewport of
 * the interface window were changed.
 *
 * @param  coordinate [description]
 * @return            [description]
 */
Coordinate DrawingArea::convertCoordinateToViewPort(Coordinate &coordinate)
{
  big_double xC = coordinate.x;
  big_double yC = coordinate.y;

  big_double xWmin = this->viewWindow.xMin;
  big_double yWmin = this->viewWindow.yMin;
  big_double xWmax = this->viewWindow.xMax;
  big_double yWmax = this->viewWindow.yMax;

  big_double xVpmin = this->viewPort.xMin;
  big_double yVpmin = this->viewPort.yMin;
  big_double xVpmax = this->viewPort.xMax;
  big_double yVpmax = this->viewPort.yMax;

  // double     x=((c.x-wmin.x) / (wmax.x-wmin.x)) *_width;
  big_double xVp =((xC - xWmin) / (xWmax - xWmin)) * (xVpmax - xVpmin);

  // double     y = (1 -((c.y -wmin.y) /(wmax.y -wmin.y))) *_height;
  big_double yVp = (1.0 - ((yC - yWmin) / (yWmax - yWmin))) * (yVpmax - yVpmin);

  // std::cout << "transformCoordinate: " << Coordinate(xVp, yVp);
  // std::cout << ", Original: " << coordinate << std::endl;
  // std::cout << ", wmin x: " << xWmin << ", y: " << yWmin;
  // std::cout << ", wmax x: " << xWmax << ", y: " << yWmax;
  // std::cout << ", _width x: " << xVpmax - xVpmin << ", _height: " << yVpmax - yVpmin << std::endl;
  return Coordinate(xVp, yVp);
}

void DrawingArea::updateViewPortSize(big_double width, big_double height)
{
  LOG(8, "Entering: %s; Allocation: %sx%s", *this, width, height);

  // This is true only when you resize the you DrawingArea widget window
  if (this->viewPort.xMax != width || this->viewPort.yMax != height)
  {
    big_double widthDiff  = width  - (this->viewPort.xMax - this->viewPort.xMin);
    big_double heightDiff = height - (this->viewPort.yMax - this->viewPort.yMin);

    // Keep the window zoom size while resizing the window
    // // On the first time we run this algorithm, the ViewPort.xMax is set to zero. Therefore, we must
    // // to initialize this within the current size of the DrawingArea
    // if (this->viewPort.xMax != 0)
    // {
    //   this->viewWindow.xMax = this->viewWindow.xMax + (this->viewWindow.xMax - this->viewWindow.xMin)
    //       * widthDiff / (this->viewPort.xMax - this->viewPort.xMin);
    // }
    // else
    // {
    //   this->viewWindow.xMax = widthDiff;
    // }

    // if (this->viewPort.yMax != 0)
    // {
    //   this->viewWindow.yMin = this->viewWindow.yMin - (this->viewWindow.yMax - this->viewWindow.yMin)
    //       * (heightDiff / (this->viewPort.yMax - this->viewPort.yMin));
    // }
    // else
    // {
    //   // LOG(8, "If we exchange this `viewWindow.yMax` to `viewWindow.yMin` our world becomes up-side-down");
    //   this->viewWindow.yMax = heightDiff;
    // }

    this->viewPort.xMax += widthDiff;
    this->viewPort.yMax += heightDiff;

    this->viewWindow.callObservers();
    LOG(8, "Leaving:  %s %s", *this, this->viewPort);
  }
}

Signal<>::Connection DrawingArea::addObserver(const Signal<>::Callback &callback)
{
  return this->callObservers.connect(callback);
}

void DrawingArea::addLine(std::string name, int x1_cord, int y1_cord, int x2_cord, int y2_cord)
{
  Coordinate* point_cord1 = new Coordinate(x1_cord, y1_cord);
  Coordinate* point_cord2 = new Coordinate(x2_cord, y2_cord);

  Line* line = new Line(name, point_cord1, point_cord2);
  this->addObject(line);
}

void DrawingArea::addPoint(std::string name, int x_coord, int y_coord)
{
  Coordinate* point_cord = new Coordinate(x_coord, y_coord);
  Point* point = new Point(name, point_cord);
  this->addObject(point);
}

void DrawingArea::addPolygon(std::string name, std::vector<big_double> polygon_coord_list)
{
  int unsigned coordinates_size = polygon_coord_list.size();
  std::list<Coordinate*> coordinates;

  for( unsigned int index = 2; index < coordinates_size; index++, index++, index++ )
  {
    LOG(1, "Currently we do not support 3D, forcing z `%s` to be 1", polygon_coord_list.at(index-2));
    coordinates.push_back( new Coordinate( polygon_coord_list.at(index-2), polygon_coord_list.at(index-1), 1 ) );
  }

  Polygon* polygon = new Polygon(name, coordinates);
  this->addObject(polygon);
}

void DrawingArea::addObject(DrawableObject* object)
{
  if (object->getWorldCoordinates().size() == 0)
  {
    LOG(1, "");
    LOG(1, "");
    LOG(1, "ERROR: The object `%s` has no coordinates.", *object);
  }
  else
  {
    LOG(4, "Adding the object `%s`", *object);
  }

  object->updateWindowCoordinates(this->viewWindow);
  object->updateClippingCoordinates(this->viewPort);

  this->displayFile.addObject(object);
  this->callObservers();
}

void DrawingArea::removeObject(std::string name)
{
  // LOG(4, "Removing an object by name is faster than by pointer because it internally calls `removeObjectByName()`");
  this->displayFile.removeObjectByName(name);
  this->callObservers();
}

void DrawingArea::updateObjectCoordinates()
{
  LOG(4, "Updating all objects clipping...");
  auto objects = this->displayFile.getObjects();

  for (auto object : objects)
  {
    object->updateWindowCoordinates(this->viewWindow);
    object->updateClippingCoordinates(this->viewPort);
  }

  this->queue_draw();
}

std::list<std::string> DrawingArea::getNamesList()
{
  return this->displayFile.getNamesList();
}

std::list<DrawableObject*> DrawingArea::getObjectsList()
{
  return this->displayFile.getObjects();
}

void DrawingArea::move(big_double horizontal_step, big_double vertical_step)
{
  this->viewWindow.move(Coordinate(horizontal_step, vertical_step));
}

void DrawingArea::zoom(big_double step)
{
  this->viewWindow.zoom(Coordinate(step, step));
}

void DrawingArea::rotate(Coordinate angle)
{
  this->viewWindow.rotate(angle);
}
