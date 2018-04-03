/*
 * RwObjectService.h
 *
 *  Created on: 23 de mar de 2018
 *      Author: karla
 */

#ifndef RWOBJECTSERVICE_H_
#define RWOBJECTSERVICE_H_

#include <list>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#include "facade.h"
#include "drawableobject.h"

class RwObjectService
{
public:
  RwObjectService(Facade&);
  ~RwObjectService();

  void read(std::string file_path);
  void write(std::list<DrawableObject*> objects_list, std::string file_path);

protected:
  Facade& facade;
  std::vector<std::string> split(std::string str, char delimiter);
};

#endif /* RWOBJECTSERVICE_H_ */
