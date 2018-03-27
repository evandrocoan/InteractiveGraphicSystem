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

void ViewPort::on_init(){

  auto allocation = this->get_allocation();
  this->updateViewport(allocation);

  Coordinate windowCenter(this->xVpmax/2, this->yVpmax/2);
  this->viewWindow.setCoordinate(windowCenter); 
  LOG(4, "centerWindow: %d , %d",this->xVpmax/2, this->yVpmax/2);

  Coordinate* bottom_axe = new Coordinate(0, -MAX_HEIGHT);
  Coordinate* top_axe = new Coordinate(0, MAX_WIDTH);

  Coordinate bottom_axe_converted = this->coordinateWorldToWindow(*bottom_axe);
  Coordinate top_axe_converted = this->coordinateWorldToWindow(*top_axe);

  Coordinate* bottom_axe_in_window = new Coordinate(bottom_axe_converted.getx(), bottom_axe_converted.gety());
  Coordinate* top_axe_in_window = new Coordinate(top_axe_converted.getx(), top_axe_converted.gety());



  Coordinate* left_axe = new Coordinate(-MAX_HEIGHT, 0);
  Coordinate* right_axe = new Coordinate(MAX_WIDTH, 0);

  Coordinate left_axe_converted = this->coordinateWorldToWindow(*left_axe);
  Coordinate right_axe_converted = this->coordinateWorldToWindow(*right_axe);

  Coordinate* left_axe_in_window = new Coordinate(left_axe_converted.getx(), left_axe_converted.gety());
  Coordinate* right_axe_in_window = new Coordinate(right_axe_converted.getx(), right_axe_converted.gety());

  Line* lineY = new Line("Y axe", bottom_axe, top_axe, bottom_axe_in_window, top_axe_in_window);
  Line* lineX = new Line("X axe", left_axe, right_axe, left_axe_in_window, right_axe_in_window);
  
  this->addObject(lineY);
  this->addObject(lineX);

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

  if(this->onInit){
    this->on_init();
    this->onInit = false;
  }

  // LOG(8, "Paint white background");
  cairo_context->set_source_rgb(1, 1, 1);
  cairo_context->paint();

  // LOG(8, "Draw x and y axis");
  cairo_context->set_line_width(1);
  cairo_context->set_source_rgb(0.741176, 0.717647, 0.419608);

  // LOG(8, "Set color's objects as black:");
  cairo_context->set_source_rgb(0, 0, 0);

  // LOG(8, "Draw displayFile objects");
  auto objects = this->displayFile.getObjects();

  for (auto object : objects)
  {
    auto coordinates = object->getviewWindowCoordinates();

    Coordinate firstCoordinate = this->coordinateWindowToViewPort(**(coordinates.begin()));

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
        Coordinate coordinateConverted = this->coordinateWindowToViewPort(*coordinate);
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

Coordinate ViewPort::coordinateWindowToViewPort(Coordinate &coord)
{
  Coordinate centerWindow = this->viewWindow.getCoordinate();
  Coordinate centerWindow_converted = this->convertCoordinateFromWindow(centerWindow);

  long int x = coord.getx() + centerWindow_converted.getx();
  long int y = -coord.gety() + centerWindow_converted.gety();


  return Coordinate(x, y);
}

Coordinate ViewPort::coordinateWorldToWindow(Coordinate &coord)
{

  Coordinate coordinate = this->convertCoordinateFromWindow(coord);

  Coordinate centerWindow = this->viewWindow.getCoordinate();
  Coordinate centerWindow_converted = this->convertCoordinateFromWindow(centerWindow);

  long int x = coordinate.getx() - centerWindow_converted.getx();
  long int y = -(coordinate.gety() - centerWindow_converted.gety());


  return Coordinate(x, y);
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

    this->viewWindow.initPoints();
    this->viewWindow.setPoints();

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
  Transformation transformation;
  Coordinate center = this->viewWindow.getCoordinate();
  transformation.add_translation("windowUp",Coordinate(0,-length));
  transformation.set_geometric_center(center);

  this->computeWindowCoordinate(transformation);
  this->viewWindow.move_up(length);

  this->queue_draw();
}

void ViewPort::move_down(int length)
{
  Transformation transformation;
  Coordinate center = this->viewWindow.getCoordinate();
  transformation.add_translation("windowDown",Coordinate(0,length));
  transformation.set_geometric_center(center);

  this->computeWindowCoordinate(transformation);
  this->viewWindow.move_down(length);

  this->queue_draw();
}

void ViewPort::move_left(int length)
{
  Transformation transformation;
  Coordinate center = this->viewWindow.getCoordinate();
  transformation.add_translation("windowLeft",Coordinate(length,0));
  transformation.set_geometric_center(center);

  this->computeWindowCoordinate(transformation);
  this->viewWindow.move_left(length);

  this->queue_draw();
}

void ViewPort::move_right(int length)
{
  Transformation transformation;
  Coordinate center = this->viewWindow.getCoordinate();
  transformation.add_translation("windowRight",Coordinate(-length,0));
  transformation.set_geometric_center(center);

  this->computeWindowCoordinate(transformation);
  this->viewWindow.move_right(length);

  this->queue_draw();
}

void ViewPort::move_center()
{
  Transformation transformation;
  Coordinate center = this->viewWindow.getCoordinate();
  transformation.add_translation("windowCenter",Coordinate(this->viewWindow.getCoordinate().getx(), this->viewWindow.getCoordinate().gety()));
  transformation.set_geometric_center(center);

  this->computeWindowCoordinate(transformation);

  this->viewWindow.move_center();
  this->queue_draw();
}

void ViewPort::rotate_left(long double angle)
{
  Transformation transformation;
  Coordinate center = this->viewWindow.getCoordinate();
  Coordinate centerWorld = Coordinate(0,0);
  
  transformation.add_translation("Center with the world",Coordinate(-center.getx(), -center.gety()));
  transformation.add_rotation("Window Rotation", Array<3, long double>{angle, 0.0, 0.0});
  transformation.add_translation("Back to initial position",Coordinate(center.getx(), center.gety()));
  transformation.set_geometric_center(centerWorld);

  this->computeWindowCoordinate(transformation);
  this->viewWindow.rotate_left(angle);

  this->queue_draw();
}

void ViewPort::rotate_right(long double angle)
{
  Transformation transformation;
  Coordinate center = this->viewWindow.getCoordinate();
  Coordinate centerWorld = Coordinate(0,0);
  
  transformation.add_translation("Center with the world",Coordinate(-center.getx(), -center.gety()));
  transformation.add_rotation("Window Rotation", Array<3, long double>{-angle, 0.0, 0.0});
  transformation.add_translation("Back to initial position",Coordinate(center.getx(), center.gety()));
  transformation.set_geometric_center(centerWorld);

  this->computeWindowCoordinate(transformation);
  this->viewWindow.rotate_right(angle);

  this->queue_draw();
}

void ViewPort::computeWindowCoordinate(Transformation transformation){

auto objects = this->displayFile.getObjects();

  for (auto object : objects)
  {
   object->applyInWindow(transformation); 
  }


}