/*
 * RwObjectService.h
 *
 *  Created on: 23 de mar de 2018
 *      Author: karla
 *
 *  Refactored on: 09 de nov de 2018
 *      Author: evandro
 */

#ifndef RWOBJECTSERVICE_H_
#define RWOBJECTSERVICE_H_

#include <list>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>

#include "facade.h"
#include "drawableobject.h"

class RwObjectService
{
public:
  RwObjectService(Facade&);
  ~RwObjectService();

  void read(std::string file_path);
  void write(std::vector<DrawableObject*> objects_list, std::string file_path);

protected:
  Facade& facade;
  std::vector<Coordinate*> getVertexes(std::vector<int>&, std::vector<big_double>&);

  std::vector<int> getLineIndexes(std::vector<int>&, std::string&);
  std::vector<std::string> split(std::string&, char delimiter);
};

#endif /* RWOBJECTSERVICE_H_ */
