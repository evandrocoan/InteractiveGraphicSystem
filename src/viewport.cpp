#include "viewport.h"

Viewport::Viewport() :
      viewwindow(new Viewwindow (0, 0, 0, 0)),
      Xvpmin(0),
      Yvpmin(0),
      Xvpmax(0),
      Yvpmax(0)
{
}

/**
 * [Viewport::on_draw description]
 *
 * @param `cairo_context` Context is the main class used to draw in cairomm. It contains the current
 *     state of the rendering device, including coordinates of yet to be drawn shapes.
 *
 * @return               [description]
 */
bool Viewport::on_draw(const Cairo::RefPtr<Cairo::Context>& cairo_context)
{
  this->updateViewport(this->get_allocation());

  LOG(8, "Paint white background");
  cairo_context->set_source_rgb(1, 1, 1);
  cairo_context->paint();

  LOG(8, "Draw x and y axis");
  cairo_context->set_line_width(1);
  cairo_context->set_source_rgb(0.741176, 0.717647, 0.419608);
  Coordinate originOnWindow = convertCoordinateFromWindow(Coordinate(0, 0));

  cairo_context->move_to(Xvpmin, originOnWindow.gety());
  cairo_context->line_to(Xvpmax, originOnWindow.gety());
  cairo_context->move_to(originOnWindow.getx(), Yvpmin);
  cairo_context->line_to(originOnWindow.getx(), Yvpmax);
  cairo_context->stroke();

  LOG(8, "Set color's objects as black:");
  cairo_context->set_source_rgb(0, 0, 0);

  LOG(8, "Draw displayfile objects");
  std::list<DrawableObject*> objects = this->getDisplayFile()->getObjects();

  for (std::list<DrawableObject*>::iterator it_obj = objects.begin(); it_obj != objects.end(); it_obj++)
  {
    list<Coordinate*> objectCoordinates = (*it_obj)->getCoordinates();

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
        Coordinate cordConverted = this->convertCoordinateFromWindow(**it_cord);
        cairo_context->line_to(cordConverted.getx(),cordConverted.gety());
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
Coordinate Viewport::convertCoordinateFromWindow(Coordinate cord)
{
  long int Xw = cord.getx();
  long int Xvp = (long int)(
      (double)(Xw - this->viewwindow->getXwmin()) * ((double)(this->Xvpmax - this->Xvpmin) /
          (double)(this->viewwindow->getXwmax() - this->viewwindow->getXwmin())
      )
  );

  long int Yw = cord.gety();
  long int Yvp = (this->Yvpmax - this->Yvpmin) - (long int)(
      (double)(Yw - this->viewwindow->getYwmin()) * (double)(this->Yvpmax - this->Yvpmin) /
          (double)(this->viewwindow->getYwmax() - this->viewwindow->getYwmin())
  );

  return Coordinate(Xvp, Yvp);
}

/**
 * Resize viewwindow when viewport is resized.
 *
 * Gtk::Allocation is a typedef of Gdk::Rectangle because GtkAllocation is a typedef of GdkRectangle.
 *
 * @param allocation is a structure holding the position and size of a rectangle. The intersection
 *     of two rectangles can be computed with intersect(). To find the union of two rectangles use
 *     join().
 */
void Viewport::updateViewport(Gtk::Allocation allocation)
{
  // NÃO ENTENDI A LÓGICA MATEMÁTICA
  if (this->Xvpmax != allocation.get_width() ||  this->Yvpmax != allocation.get_height())
  {
    float xwmax;

    int widthDiff  = allocation.get_width()  - (this->Xvpmax - this->Xvpmin);
    int heightDiff = allocation.get_height() - (this->Yvpmax - this->Yvpmin);

    if (this->Xvpmax != 0)
    {
      xwmax = this->viewwindow->getXwmax()
          + (float)(this->viewwindow->getXwmax() - this->viewwindow->getXwmin()) * ( (float)widthDiff
              / (float)(this->Xvpmax - this->Xvpmin)
          );
    }
    else
    {
      xwmax = (float)widthDiff;
    }

    this->viewwindow->setXwmax( xwmax );

    if (this->Yvpmax != 0)
    {
      this->viewwindow->setYwmin(
          this->viewwindow->getYwmin()
              - (float)(this->viewwindow->getYwmax()
                  - this->viewwindow->getYwmin()
              ) * ((float)heightDiff / (float)(this->Yvpmax - this->Yvpmin))
      );
    }
    else
    {
      LOG(8, "If we exchange this to `setYwmin()` our world becomes up-side-down");
      this->viewwindow->setYwmax((float)heightDiff);
    }

    this->Xvpmax += widthDiff;
    this->Yvpmax += heightDiff;
    LOG(8, "Xvpmax: %d, Yvpmax: %d", Xvpmax, Yvpmax);
  }
}

Viewport::~Viewport()
{
}

Viewwindow* Viewport::getViewwindow()
{
  return this->viewwindow;
}

DisplayFile* Viewport::getDisplayFile()
{
  return &this->displayFile;
}

