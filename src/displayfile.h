#ifndef GTKMM_APP_DISPLAY_FILE
#define GTKMM_APP_DISPLAY_FILE

#include <map>
#include <list>
#include <string>
#include "drawableobject.h"

class DisplayFile
{
public:
  DisplayFile();
  virtual ~DisplayFile();

  void addObject(DrawableObject* object);
  void removeObject(DrawableObject* object);
  void removeObjectByName(string name);

  std::list<string> getNamesList();
  std::list<DrawableObject*> getObjects();

  DrawableObject* getObjectByName(string name);

protected:
  std::map<string, DrawableObject*> objectsMap;

};

#endif
