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

#include "drawingarea.h"
#include "drawableobject.h"

using namespace std;

class RwObjectService
{
public:
  RwObjectService(DrawingArea&);
  ~RwObjectService();

  void read(string file_path);
  void write(list<DrawableObject*> objects_list, string file_path);

protected:
  DrawingArea& drawingArea;
  vector<string> split(string str, char delimiter);
};

#endif /* RWOBJECTSERVICE_H_ */