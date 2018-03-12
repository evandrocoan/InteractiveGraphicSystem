#ifndef GTKMM_APP_WIREFRAME
#define GTKMM_APP_WIREFRAME

#include "drawableobject.h"
#include "coordinate.h"

using namespace std;

class Polygon : public DrawableObject
{
public:
  Polygon(string name, list<Coordinate*> coordinates);
  ~Polygon();
};
#endif // GTKMM_APP_WIREFRAME
