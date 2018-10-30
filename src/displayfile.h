#ifndef GTKMM_APP_DISPLAY_FILE
#define GTKMM_APP_DISPLAY_FILE

#include <map>
#include <list>
#include <string>

#include "drawableobject.h"
class World;

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

  void removeObjectByName(std::string name);
  DrawableObject* apply(std::string object_name, Transformation&);

  bool            isObjectOnByName(std::string name);
  DrawableObject* getObjectByName(std::string name);

  std::vector<std::string> objectNames() const;
  std::vector<DrawableObject*> getObjects() const;

protected:
  std::map<std::string, DrawableObject*> objectsMap;
};

#endif // GTKMM_APP_DISPLAY_FILE
