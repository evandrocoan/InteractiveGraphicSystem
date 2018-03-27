/*
 * RwObjectService.cpp
 *
 *  Created on: 23 de mar de 2018
 *      Author: karla
 */

#include "RwObjectService.h"

RwObjectService::RwObjectService()
{
}

RwObjectService::~RwObjectService()
{
}

list<DrawableObject*> RwObjectService::read(string file_path)
{
  list<DrawableObject*> objects_list;
  string line;
  ifstream myfile(file_path);
  list<Coordinate*> cord_list;
  string name;
  if(myfile.is_open())
  {
    while(getline(myfile, line))
    {
      if (line.front() == 'o')
      {
        name = line.substr(2, line.length());
      }
      if (line.front() == 'v')
      {
        vector<string> sep = split(line, ' ');
        Coordinate* cord;
        if (sep.size() > 3)
        {
          cord = new Coordinate(atof(sep[1].c_str()), atof(sep[2].c_str()), atof(sep[3].c_str()));
        }
        else
        {
          cord = new Coordinate(atof(sep[1].c_str()), atof(sep[2].c_str()));
        }
        cord_list.push_back(cord);
      }
      if (line.front() == 'f')
      {
        DrawableObject *object;
        if (cord_list.size() == 1)
        {
          object = new Point(name, cord_list.front(), cord_list.front());
        }
        else if (cord_list.size() == 2)
        {
          object = new Line(name, cord_list.front(), cord_list.back(), cord_list.front(), cord_list.back());
        }
        else
        {
          object = new Polygon(name, cord_list, cord_list);
        }
        objects_list.push_back(object);
        cord_list = * new list<Coordinate*>;
      }
    }
    myfile.close();
  }
  return objects_list;
}

void RwObjectService::write(list<DrawableObject*> objects_list, string file_path)
{
  ofstream myfile;
  myfile.open(file_path);
  myfile << "# Starting the file objects\n\n";
  for (list<DrawableObject*>::iterator it_obj = objects_list.begin();
        it_obj != objects_list.end(); ++it_obj)
  {
    myfile << "o " + (*it_obj)->getName() + "\n\n";
    list<Coordinate*> objectCoordinates = (*it_obj)->getCoordinates();
    for (std::list<Coordinate*>::iterator it_cord = objectCoordinates.begin(); it_cord != objectCoordinates.end(); ++it_cord)
    {
      string line_cord = "v " + to_string((*it_cord)->getx()) + " " + to_string((*it_cord)->gety())
      + " " + to_string((*it_cord)->getz()) + "\n";
      myfile << line_cord;
    }
    myfile << "\nf ";
    for (unsigned int i = 1; i < objectCoordinates.size()+1; i++)
    {
      myfile << to_string(i) + " ";
    }
    myfile << "\n\n";
  }
  myfile.close();
}

vector<string> RwObjectService::split(string str, char delimiter)
{
  vector<string> internal;
  stringstream ss(str); // Turn the string into a stream.
  string tok;

  while(getline(ss, tok, delimiter)) {
    internal.push_back(tok);
  }

  return internal;
}
