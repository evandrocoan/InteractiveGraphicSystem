/*
 * RwObjectService.cpp
 *
 *  Created on: 23 de mar de 2018
 *      Author: karla
 */

#include "RwObjectService.h"

RwObjectService::RwObjectService(Facade& facade) :
      facade(facade)
{
}

RwObjectService::~RwObjectService()
{
}

void RwObjectService::read(std::string file_path)
{
  std::string line;
  std::ifstream myfile(file_path);
  std::vector<big_double> coord_vector;
  std::string name;

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
        std::vector<std::string> sep = split(line, ' ');

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
          this->facade.addPoint(name, coord_vector[0], coord_vector[1]);
        }
        else if (coord_vector.size() == 2)
        {
          this->facade.addLine(name, coord_vector[0], coord_vector[1], coord_vector[2], coord_vector[3]);
        }
        else
        {
          this->facade.addPolygon(name, coord_vector);
        }

        coord_vector.clear();
      }
    }

    myfile.close();
  }
}

void RwObjectService::write(std::vector<DrawableObject*> objects_list, std::string file_path)
{
  std::ofstream myfile;
  myfile.open(file_path);
  myfile << "# Starting the file objects\n\n";

  for (auto it_obj = objects_list.begin(); it_obj != objects_list.end(); ++it_obj)
  {
    myfile << "o " + (*it_obj)->getName() + "\n\n";
    std::vector<Coordinate*> objectCoordinates = (*it_obj)->worldCoordinates();

    for (std::vector<Coordinate*>::iterator it_cord = objectCoordinates.begin(); it_cord != objectCoordinates.end(); ++it_cord)
    {
      std::string line_cord = "v " + std::to_string((*it_cord)->x) + " " + std::to_string((*it_cord)->y)
          + " " + std::to_string((*it_cord)->z) + "\n";

      myfile << line_cord;
    }

    myfile << "\nf ";

    for (unsigned int i = 1; i < objectCoordinates.size()+1; i++)
    {
      myfile << std::to_string(i) + " ";
    }

    myfile << "\n\n";
  }

  myfile.close();
}

std::vector<std::string> RwObjectService::split(std::string str, char delimiter)
{
  std::vector<std::string> internal;
  std::stringstream ss(str); // Turn the std::string into a stream.
  std::string tok;

  while(getline(ss, tok, delimiter)) {
    internal.push_back(tok);
  }

  return internal;
}
