#include "viewport.h"

ViewPort::ViewPort() :
      viewWindow(0, 0, 0, 0),
      displayFile(),
      isCentered(false),
      xVpmin(0),
      yVpmin(0),
      xVpmax(0),
      yVpmax(0)
{
  this->signal_size_allocate().connect(sigc::mem_fun(*this, &ViewPort::on_my_size_allocate));
}

void ViewPort::on_my_size_allocate(Gtk::Allocation& allocation)
{
  if( !isCentered )
  {
    int width = this->get_width();
    int height = this->get_height();

    this->isCentered = true;
    LOG(4, "Moving ViewWindow (0, 0) to the window center (%s, %d)...", width, height);

    this->move_down(480);
    this->move_left(500);
  }
}

void ViewPort::apply(std::string object_name, Transformation &transformation)
{
  if( this->displayFile.isObjectOnByName(object_name) )
  {
    if( transformation.size() )
    {
      this->displayFile.apply(object_name, transformation);
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

/**
 * [ViewPort::on_draw description]
 *
 * @param `cairo_context` Context is the main class used to draw in cairomm. It contains the current
 *     state of the rendering device, including coordinates of yet to be drawn shapes.
 *
 * @return               [description]
 */
bool ViewPort::on_draw(const Cairo::RefPtr<Cairo::Context>& cairo_context)
{
  // LOG(8, "Chama-mes 5 vezes seguidas para desenhar a mesma coisa por que?");
  auto allocation = this->get_allocation();
  this->updateViewport(allocation);

  // LOG(8, "Paint white background");
  cairo_context->set_source_rgb(1, 1, 1);
  cairo_context->paint();

  cairo_context->set_line_width(1);
  cairo_context->set_source_rgb(0.741176, 0.717647, 0.419608);
  Coordinate originOnWindow(0, 0);
  Coordinate originOnWorld = convertCoordinateFromWindow(originOnWindow);

  LOG(8, "Draw x and y axis: %s", originOnWorld);
  cairo_context->move_to(this->xVpmin, originOnWorld.gety());
  cairo_context->line_to(this->xVpmax, originOnWorld.gety());
  cairo_context->move_to(originOnWorld.getx(), this->yVpmin);
  cairo_context->line_to(originOnWorld.getx(), this->yVpmax);
  cairo_context->stroke();

  // LOG(8, "Set color's objects as black:");
  cairo_context->set_source_rgb(0, 0, 0);

  // LOG(8, "Draw displayFile objects");
  auto objects = this->displayFile.getObjects();

  for (auto object : objects)
  {
    auto coordinates = object->getCoordinates();
    Coordinate firstCoordinate = this->convertCoordinateFromWindow(**(coordinates.begin()));

    cairo_context->move_to(firstCoordinate.getx(), firstCoordinate.gety());
    LOG(8, "object coordinates: %s", *object);

    if (coordinates.size() == 1)
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
 * @param  coord [description]
 * @return       [description]
 */
Coordinate ViewPort::convertCoordinateFromWindow(Coordinate &coord)
{
  long int xW = coord.getx();
  long int xVp = (long int)(
      (double)(xW - this->viewWindow.xWmin) * ((double)(this->xVpmax - this->xVpmin) /
          (double)(this->viewWindow.xWmax - this->viewWindow.xWmin)
      )
  );

  long int yW = coord.gety();
  long int yVp = (this->yVpmax - this->yVpmin) - (long int)(
      (double)(yW - this->viewWindow.yWmin) * (double)(this->yVpmax - this->yVpmin) /
          (double)(this->viewWindow.yWmax - this->viewWindow.yWmin)
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
 * @param `allocation` is a structure holding the position and size of a rectangle. The intersection
 *     of two rectangles can be computed with intersect(). To find the union of two rectangles use
 *     join(). Gtk::Allocation is a typedef of Gdk::Rectangle because GtkAllocation is a typedef of
       GdkRectangle.
 */
void ViewPort::updateViewport(Gtk::Allocation &allocation)
{
  // NÃO ENTENDI A LÓGICA MATEMÁTICA
  if (this->xVpmax != allocation.get_width() ||  this->yVpmax != allocation.get_height())
  {
    float xWmax;

    int widthDiff  = allocation.get_width()  - (this->xVpmax - this->xVpmin);
    int heightDiff = allocation.get_height() - (this->yVpmax - this->yVpmin);

    if (this->xVpmax != 0)
    {
      xWmax = this->viewWindow.xWmax
          + (float)(this->viewWindow.xWmax - this->viewWindow.xWmin) * ( (float)widthDiff
              / (float)(this->xVpmax - this->xVpmin)
          );
    }
    else
    {
      xWmax = (float)widthDiff;
    }

    this->viewWindow.xWmax = xWmax;

    if (this->yVpmax != 0)
    {
      this->viewWindow.yWmin = (
          this->viewWindow.yWmin
              - (float)(this->viewWindow.yWmax
                  - this->viewWindow.yWmin
              ) * ((float)heightDiff / (float)(this->yVpmax - this->yVpmin))
      );
    }
    else
    {
      LOG(8, "If we exchange this to `setYwmin()` our world becomes up-side-down");
      this->viewWindow.yWmax = (float)heightDiff;
    }

    this->xVpmax += widthDiff;
    this->yVpmax += heightDiff;
    LOG(8, "xVpmax: %d, yVpmax: %d", xVpmax, yVpmax);
  }
}

ViewPort::~ViewPort()
{
}

Signal<>::Connection ViewPort::addObserver(const Signal<>::Callback &callback)
{
    return observerController.connect(callback);
}

void ViewPort::addObject(DrawableObject* object)
{
  this->displayFile.addObject(object);
  this->queue_draw();
  this->observerController();
}

void ViewPort::removeObject(std::string name)
{
  this->displayFile.removeObjectByName(name);
  this->queue_draw();
  this->observerController();
}

std::list<std::string> ViewPort::getNamesList()
{
  return this->displayFile.getNamesList();
}

void ViewPort::zoom_in(float scale)
{
  this->viewWindow.zoom_in(scale);
  this->queue_draw();
}

void ViewPort::zoom_out(float scale)
{
  this->viewWindow.zoom_out(scale);
  this->queue_draw();
}

void ViewPort::move_up(int length)
{
  this->viewWindow.move_up(length);
  this->queue_draw();
}

void ViewPort::move_down(int length)
{
  this->viewWindow.move_down(length);
  this->queue_draw();
}

void ViewPort::move_left(int length)
{
  this->viewWindow.move_left(length);
  this->queue_draw();
}

void ViewPort::move_right(int length)
{
  this->viewWindow.move_right(length);
  this->queue_draw();
}
