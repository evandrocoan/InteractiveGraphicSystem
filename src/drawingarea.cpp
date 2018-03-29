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
      << "ViewPort" << object.viewPort << " "
      << "ViewWindow" << object.viewWindow;
  return output;
}

/**
 * Move the X and Y axes to the center of the window when the program starts and recalculate the
 * clipping window size when the window size changes.
 *
 * @param allocation [description]
 */
void DrawingArea::on_my_size_allocate(Gtk::Allocation& allocation)
{
  const int width = this->get_width();
  const int height = this->get_height();

  if( !isCentered )
  {
    this->isCentered = true;
    LOG(4, "Moving ViewWindow (0, 0) to the window center...");

    this->on_init();
    this->move_down(480);
    this->move_left(500);
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

void DrawingArea::on_init()
{
  LOG(4, "centerWindow: %d , %d",this->viewPort.xMax/2, this->viewPort.yMax/2);

  LOG(4, "Building the Y axe");
  Coordinate* bottom_axe = new Coordinate(0, -MAX_HEIGHT);
  Coordinate* top_axe = new Coordinate(0, MAX_WIDTH);

  LOG(4, "Building the X axe");
  Coordinate* left_axe = new Coordinate(-MAX_HEIGHT, 0);
  Coordinate* right_axe = new Coordinate(MAX_WIDTH, 0);

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
  auto allocation = this->get_allocation();
  this->updateViewPort(allocation);

  // LOG(8, "Paint white background");
  cairo_context->set_source_rgb(1, 1, 1);
  cairo_context->paint();

  // cairo_context->set_line_width(1);
  // cairo_context->set_source_rgb(0.741176, 0.717647, 0.419608);
  // Coordinate originOnWindow(0, 0);
  // Coordinate originOnWorld = convertCoordinateFromWindow(originOnWindow);

  // LOG(8, "Drawing X and Y axes with originOnWorld: %s", originOnWorld);
  // LOG(4, "Drawing axes X from (%s, %s) to (%s, %s)", this->viewPort.xMin, originOnWorld.gety(), this->viewPort.xMax, originOnWorld.gety() );
  // LOG(4, "Drawing axes Y from (%s, %s) to (%s, %s)", originOnWorld.getx(), this->viewPort.yMin, originOnWorld.getx(), this->viewPort.yMax );
  // cairo_context->move_to(this->viewPort.xMin + clipping_window_margin_distance, originOnWorld.gety());
  // cairo_context->line_to(this->viewPort.xMax - clipping_window_margin_distance, originOnWorld.gety());
  // cairo_context->move_to(originOnWorld.getx(), this->viewPort.yMin + clipping_window_margin_distance);
  // cairo_context->line_to(originOnWorld.getx(), this->viewPort.yMax - clipping_window_margin_distance);
  // cairo_context->stroke();

  // LOG(8, "Draw the clipping window with a red border")
  cairo_context->set_source_rgb(0.99, 0.0, 0.0);

  cairo_context->line_to(this->viewPort.getPoint(0)->getx(), this->viewPort.getPoint(0)->gety());
  cairo_context->line_to(this->viewPort.getPoint(1)->getx(), this->viewPort.getPoint(1)->gety());
  cairo_context->line_to(this->viewPort.getPoint(2)->getx(), this->viewPort.getPoint(2)->gety());
  cairo_context->line_to(this->viewPort.getPoint(3)->getx(), this->viewPort.getPoint(3)->gety());
  cairo_context->line_to(this->viewPort.getPoint(0)->getx(), this->viewPort.getPoint(0)->gety());

  // LOG(8, "Set color's objects as black:");
  cairo_context->stroke();
  cairo_context->set_source_rgb(0, 0, 0);

  // LOG(8, "Draw displayFile objects");
  auto objects = this->displayFile.getObjects();

  for (auto object : objects)
  {
    // auto coordinates = object->getViewWindowCoordinates();
    auto coordinates = object->getClippingCoordinates();
    int coordinates_count = coordinates.size();

    if (coordinates_count == 0)
    {
      LOG(1, "ERROR: The object `%s` has no coordinates.", *object);
      continue;
    }

    Coordinate firstCoordinate = this->convertCoordinateFromWindow(**(coordinates.begin()));

    LOG(8, "object coordinates: %s", *object);
    cairo_context->move_to(firstCoordinate.getx(), firstCoordinate.gety());

    if (coordinates_count == 1)
    {
      cairo_context->line_to(firstCoordinate.getx()+1, firstCoordinate.gety()+1);
    }
    else
    {
      for (auto coordinate : coordinates)
      {
        Coordinate coordinateConverted = this->convertCoordinateFromWindow(*coordinate);
        cairo_context->line_to(coordinateConverted.getx(), coordinateConverted.gety());
      }

      // LOG(8, "Draw a line until the first coordinate, closing the polygon")
      cairo_context->line_to(firstCoordinate.getx(), firstCoordinate.gety());
    }
  }

  cairo_context->stroke();
  return true;
}

/**
 * Transformada de viewport - Slide 2 de "02 - Conceitos Básicos"
 *
 * @param  coordinate [description]
 * @return            [description]
 */
Coordinate DrawingArea::convertCoordinateFromWindow(Coordinate &coordinate)
{
  long int xW = coordinate.getx();
  long int yW = coordinate.gety();

  long int xVp = (long int)(
      (double)(xW - this->viewWindow.xMin) * ((double)(this->viewPort.xMax - this->viewPort.xMin) /
          (double)(this->viewWindow.xMax - this->viewWindow.xMin)
      )
  );

  long int yVp = (this->viewPort.yMax - this->viewPort.yMin) - (long int)(
      (double)(yW - this->viewWindow.yMin) * (double)(this->viewPort.yMax - this->viewPort.yMin) /
          (double)(this->viewWindow.yMax - this->viewWindow.yMin)
  );

  return Coordinate(xVp, yVp);
}

/**
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
 * @param `allocation` is the Current DrawingArea widget size like 556x469. Allocation is a
 *     structure holding the position and size of a rectangle. The intersection of two rectangles
 *     can be computed with intersect(). To find the union of two rectangles use join().
 *     Gtk::Allocation is a typedef of Gdk::Rectangle because GtkAllocation is a typedef of
 *     GdkRectangle.
 */
void DrawingArea::updateViewPort(Gtk::Allocation &allocation)
{
  // NÃO ENTENDI A LÓGICA MATEMÁTICA.
  LOG(8, "Entering: %s; Allocation: %sx%s", *this, allocation.get_width(), allocation.get_height());

  // This is true only when you resize the you DrawingArea widget window
  if (this->viewPort.xMax != allocation.get_width() || this->viewPort.yMax != allocation.get_height())
  {
    int widthDiff  = allocation.get_width()  - (this->viewPort.xMax - this->viewPort.xMin);
    int heightDiff = allocation.get_height() - (this->viewPort.yMax - this->viewPort.yMin);

    // On the first time we run this algorithm, the ViewPort.xMax is set to zero. Therefore, we must
    // to initialize this within the current size of the DrawingArea
    if (this->viewPort.xMax != 0)
    {
      this->viewWindow.xMax = this->viewWindow.xMax
          + (float)(this->viewWindow.xMax - this->viewWindow.xMin) * ( (float)widthDiff
              / (float)(this->viewPort.xMax - this->viewPort.xMin)
          );
    }
    else
    {
      this->viewWindow.xMax = (float)widthDiff;
    }

    if (this->viewPort.yMax != 0)
    {
      this->viewWindow.yMin = (
          this->viewWindow.yMin
              - (float)(this->viewWindow.yMax
                  - this->viewWindow.yMin
              ) * ((float)heightDiff / (float)(this->viewPort.yMax - this->viewPort.yMin))
      );
    }
    else
    {
      // LOG(8, "If we exchange this `viewWindow.yMax` to `viewWindow.yMin` our world becomes up-side-down");
      this->viewWindow.yMax = (float)heightDiff;
    }

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

void DrawingArea::addPolygon(std::string name, std::vector<GTKMM_APP_MATRICES_DATATYPE> polygon_coord_list)
{
  int unsigned coordinates_size = polygon_coord_list.size();
  std::list<Coordinate*> coordinates;

  for( unsigned int index = 2; index < coordinates_size; index++, index++, index++ )
  {
    LOG(1, "Currently we do not support 3D, forcing z `%s` to be 1", polygon_coord_list.at(index-2));
    coordinates.push_back( new Coordinate( polygon_coord_list.at(index-1), polygon_coord_list.at(index), 1 ) );
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
}

std::list<std::string> DrawingArea::getNamesList()
{
  return this->displayFile.getNamesList();
}

std::list<DrawableObject*> DrawingArea::getObjectsList()
{
  return this->displayFile.getObjects();
}

void DrawingArea::zoom_in(float scale)
{
  this->viewWindow.zoom_in(scale);
  this->queue_draw();
}

void DrawingArea::zoom_out(float scale)
{
  this->viewWindow.zoom_out(scale);
  this->queue_draw();
}

void DrawingArea::move_up(int length)
{
  this->viewWindow.move_up(length);
  this->queue_draw();
}

void DrawingArea::move_down(int length)
{
  this->viewWindow.move_down(length);
  this->queue_draw();
}

void DrawingArea::move_left(int length)
{
  this->viewWindow.move_left(length);
  this->queue_draw();
}

void DrawingArea::move_right(int length)
{
  this->viewWindow.move_right(length);
  this->queue_draw();
}

void DrawingArea::rotate_left(GTKMM_APP_MATRICES_DATATYPE angle)
{
  this->viewWindow.rotate_left(angle);
  this->queue_draw();
}

void DrawingArea::rotate_right(GTKMM_APP_MATRICES_DATATYPE angle)
{
  this->viewWindow.rotate_right(angle);
  this->queue_draw();
}
