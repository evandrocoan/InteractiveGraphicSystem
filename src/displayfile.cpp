#include "displayfile.h"

DisplayFile::DisplayFile()
{
}

DisplayFile::~DisplayFile()
{
}

void DisplayFile::apply(std::string object_name, Transformation &transformation)
{
  auto object = this->getObjectByName(object_name);
  object->apply(transformation);
}

void DisplayFile::addObject(DrawableObject* object)
{
  this->objectsMap[object->getName()] = object;
}

bool DisplayFile::isObjectOnByName(std::string name)
{
  auto iterator = this->objectsMap.find(name);

  if( iterator == this->objectsMap.end() )
  {
    return false;
  }

  return true;
}

DrawableObject* DisplayFile::getObjectByName(std::string name)
{
  LOG(4, "Entering: %s", name);
  auto iterator = this->objectsMap.find(name);

  if( iterator == this->objectsMap.end() )
  {
    auto message = tfm::format("There are not objects within the name: `%s`", name); LOG(1, message.c_str());
    throw std::invalid_argument(message);
  }

  LOG(4, "Object: %s", *iterator->second);
  return iterator->second;
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
