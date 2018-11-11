/*
 * rw_object_service.h
 *
 *  Created on: 23 de mar de 2018
 *      Author: karla
 *
 *  Refactored on: 09 de nov de 2018
 *      Author: evandro
 */

#ifndef RW_OBJECT_SERVICE_H_
#define RW_OBJECT_SERVICE_H_

#include <list>
#include <regex>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>

#include "facade.h"
#include "drawableobject.h"

// https://stackoverflow.com/questions/3676664/unit-testing-of-private-methods
#include "doctest_hack.h"

class RwObjectService
{
public:
  RwObjectService(Facade&);
  ~RwObjectService();

  void read(std::string file_path);
  void write(std::string file_path);

protected:
  Facade& facade;
  std::vector<Coordinate*> getVertexes(std::vector<int>&, std::vector<big_double>&);

  std::vector<int> getLineIndexes(std::vector<int>&, std::string&);
  std::vector<std::string> split(std::string&, char delimiter);

private:
  int _last_index;
};

#endif /* RW_OBJECT_SERVICE_H_ */
