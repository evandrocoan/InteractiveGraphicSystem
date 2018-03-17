#include "viewport.h"

ViewPort::ViewPort() :
      viewWindow(0, 0, 0, 0),
      displayFile(),
      xVpmin(0),
      yVpmin(0),
      xVpmax(0),
      yVpmax(0)
{
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
  this->updateViewport(this->get_allocation());

  LOG(8, "Paint white background");
  cairo_context->set_source_rgb(1, 1, 1);
  cairo_context->paint();

  LOG(8, "Draw x and y axis");
  cairo_context->set_line_width(1);
  cairo_context->set_source_rgb(0.741176, 0.717647, 0.419608);
  Coordinate originOnWindow = convertCoordinateFromWindow(Coordinate(0, 0));

  cairo_context->move_to(this->xVpmin, originOnWindow.gety());
  cairo_context->line_to(this->xVpmax, originOnWindow.gety());
  cairo_context->move_to(originOnWindow.getx(), this->yVpmin);
  cairo_context->line_to(originOnWindow.getx(), this->yVpmax);
  cairo_context->stroke();

  LOG(8, "Set color's objects as black:");
  cairo_context->set_source_rgb(0, 0, 0);

  LOG(8, "Draw displayFile objects");
  std::list<DrawableObject*> objects = this->displayFile.getObjects();

  for (std::list<DrawableObject*>::iterator it_obj = objects.begin(); it_obj != objects.end(); it_obj++)
  {
    std::list<Coordinate*> objectCoordinates = (*it_obj)->getCoordinates();

    Coordinate firstCordConverted = this->convertCoordinateFromWindow(**(objectCoordinates.begin()));
    cairo_context->move_to(firstCordConverted.getx(),firstCordConverted.gety());

    LOG(8, "Point case, objectCoordinates: %d", objectCoordinates.size());
    if (objectCoordinates.size() == 1)
    {
      cairo_context->line_to(firstCordConverted.getx()+1,firstCordConverted.gety()+1);
    }
    else
    {
      for (std::list<Coordinate*>::iterator it_cord = objectCoordinates.begin();
              it_cord != objectCoordinates.end(); it_cord++)
      {
        Coordinate coordinateConverted = this->convertCoordinateFromWindow(**it_cord);
        cairo_context->line_to(coordinateConverted.getx(),coordinateConverted.gety());
      }

      cairo_context->line_to(firstCordConverted.getx(),firstCordConverted.gety());
    }
  }

  cairo_context->stroke();
  return true;
}

/**
 * Transformada de viewport - Slide 2 de "02 - Conceitos Básicos"
 *
 * @param  cord [description]
 * @return      [description]
 */
Coordinate ViewPort::convertCoordinateFromWindow(Coordinate cord)
{
  long int xW = cord.getx();
  long int xVp = (long int)(
      (double)(xW - this->viewWindow.xWmin) * ((double)(this->xVpmax - this->xVpmin) /
          (double)(this->viewWindow.xWmax - this->viewWindow.xWmin)
      )
  );

  long int yW = cord.gety();
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
void ViewPort::updateViewport(Gtk::Allocation allocation)
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

void ViewPort::apply(std::string object_name, Transformation transformation)
{
  this->displayFile.apply(object_name, transformation);
}

void ViewPort::addObserver(ViewPortObserver* observer)
{
  this->viewPortObservers.addObserver(observer);
}

void ViewPort::addObject(DrawableObject* object)
{
  this->displayFile.addObject(object);
  this->queue_draw();
  this->viewPortObservers.notifyObservers();
}

void ViewPort::removeObject(std::string name)
{
  this->displayFile.removeObjectByName(name);
  this->queue_draw();
  this->viewPortObservers.notifyObservers();
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
