#ifndef GTKMM_APP_DISPLAY_FILE
#define GTKMM_APP_DISPLAY_FILE

#include <map>
#include <list>
#include <string>
#include "drawableobject.h"

/**
 * `Random Scan Display` motivou o DisplayFile: Lista de todos os objetos constantemente
 * redesenhados na tela.
 */
class DisplayFile
{
public:
  DisplayFile();
  virtual ~DisplayFile();

  void addObject   (DrawableObject* object);
  void removeObject(DrawableObject* object);

  void apply(std::string object_name, Transformation);
  void removeObjectByName(std::string name);

  DrawableObject*            getObjectByName(std::string name);
  std::list<DrawableObject*> getObjects();
  std::list<std::string>     getNamesList();

protected:
  std::map<std::string, DrawableObject*> objectsMap;

};

#endif
