/*
 * RwObjectService.cpp
 *
 *  Created on: 23 de mar de 2018
 *      Author: karla
 */

#include "RwObjectService.h"

RwObjectService::RwObjectService(DrawingArea& drawingArea) :
      drawingArea(drawingArea)
{
}

RwObjectService::~RwObjectService()
{
}

void RwObjectService::read(string file_path)
{
  string line;
  ifstream myfile(file_path);
  vector<big_double> coord_vector;
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

        if (sep.size() > 3)
        {
          coord_vector.push_back(atof(sep[1].c_str()));
          coord_vector.push_back(atof(sep[2].c_str()));
          coord_vector.push_back(atof(sep[3].c_str()));
        }
        else
        {
          coord_vector.push_back(atof(sep[1].c_str()));
          coord_vector.push_back(atof(sep[2].c_str()));
          coord_vector.push_back(1.0);
        }
      }

      if (line.front() == 'f')
      {
        if (coord_vector.size() == 1)
        {
          this->drawingArea.addPoint(name, coord_vector[0], coord_vector[1]);
        }
        else if (coord_vector.size() == 2)
        {
          this->drawingArea.addLine(name, coord_vector[0], coord_vector[1], coord_vector[2], coord_vector[3]);
        }
        else
        {
          this->drawingArea.addPolygon(name, coord_vector);
        }

        coord_vector.clear();
      }
    }

    myfile.close();
  }
}

void RwObjectService::write(list<DrawableObject*> objects_list, string file_path)
{
  ofstream myfile;
  myfile.open(file_path);
  myfile << "# Starting the file objects\n\n";

  for (list<DrawableObject*>::iterator it_obj = objects_list.begin(); it_obj != objects_list.end(); ++it_obj)
  {
    myfile << "o " + (*it_obj)->getName() + "\n\n";
    list<Coordinate*> objectCoordinates = (*it_obj)->getWorldCoordinates();

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
