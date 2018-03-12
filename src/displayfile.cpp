#include "displayfile.h"

DisplayFile::DisplayFile()
{
}

DisplayFile::~DisplayFile()
{
}

void DisplayFile::addObject(DrawableObject* object)
{
  this->objectsMap[object->getName()] = object;
}

DrawableObject* DisplayFile::getObjectByName(std::string name)
{
  return this->objectsMap.find(name)->second;
}

void DisplayFile::removeObject(DrawableObject* object)
{
  this->removeObjectByName(object->getName());
}

void DisplayFile::removeObjectByName(std::string name)
{
  this->objectsMap.erase(name);
}

std::list<std::string> DisplayFile::getNamesList()
{
  std::list<std::string> list;

  for(std::map<std::string, DrawableObject*>::iterator it = objectsMap.begin(); it != objectsMap.end(); it++)
  {
    list.push_back(it->first);
  }

  return list;
}

std::list<DrawableObject*> DisplayFile::getObjects()
{
  std::list<DrawableObject*> list;

  for(std::map<std::string, DrawableObject*>::iterator it = objectsMap.begin();
      it != objectsMap.end(); it++)
  {
    list.push_back(it->second);
  }

  return list;
}
