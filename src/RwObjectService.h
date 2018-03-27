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
#include "drawableobject.h"
#include "polygon.h"
#include "point.h"
#include "line.h"

using namespace std;

class RwObjectService
{
public:
  RwObjectService();
  ~RwObjectService();
  list<DrawableObject*> read(string file_path);
  void write(list<DrawableObject*> objects_list, string file_path);
protected:
  vector<string> split(string str, char delimiter);
};

#endif /* RWOBJECTSERVICE_H_ */
