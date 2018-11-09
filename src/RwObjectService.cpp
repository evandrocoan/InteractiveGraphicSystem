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
  bool is_to_skip_new_line_fetch = false;

  bool is_there_new_lines;
  bool is_there_a_next_line;

  std::ifstream myfile(file_path);
  std::vector<big_double> coordinates;
  std::string name;

  // removes all comments and empty lines

  if( myfile.is_open() )
  {
    while( true )
    {
      if( is_to_skip_new_line_fetch ) is_to_skip_new_line_fetch = false;
          else is_there_new_lines = static_cast<bool>( getline( myfile, line ) );

      if( !is_there_new_lines ) break;

      // https://www.fileformat.info/format/wavefrontobj/egff.htm
      if( line.front() == 'o' )
      {
        name = line.substr(2, line.length());
      }
      else if( line.front() == 'v' )
      {
        std::vector<std::string> indexes = split(line, ' ');

        if( indexes.size() > 2 )
        {
          coordinates.push_back( atof( indexes[1].c_str() ) );
          coordinates.push_back( atof( indexes[2].c_str() ) );
          coordinates.push_back( atof( indexes[3].c_str() ) );
        }
        else
        {
          coordinates.push_back( atof( indexes[1].c_str() ) );
          coordinates.push_back( atof( indexes[2].c_str() ) );
          coordinates.push_back( 1.0 );
        }
      }
      else if( line.front() == 'p' )
      {
        std::vector<int> indexes;
        this->getLineIndexes( indexes, line );

        std::vector<Coordinate*> vertexes = this->getVertexes( indexes, coordinates );
        this->facade.addPoint( name, vertexes[0]->x, vertexes[1]->y );
      }
      else if( line.front() == 'l' )
      {
        std::vector<int> indexes;

        do
        {
          this->getLineIndexes( indexes, line );

          is_there_new_lines = static_cast<bool>( getline( myfile, line ) );
          is_there_a_next_line = line.front() == 'l';

          is_to_skip_new_line_fetch = true;
          if( !is_there_new_lines ) break;
        }
        while( is_there_a_next_line );

        std::vector<Coordinate*> vertexes = this->getVertexes( indexes, coordinates );

        if( vertexes.size() < 7 )
        {
          this->facade.addLine( name, vertexes[0]->x, vertexes[0]->y, vertexes[1]->x, vertexes[1]->y );
        }
        else
        {
          this->facade.addPolygon( name, vertexes );
        }
      }
    }

    myfile.close();
  }
}

std::vector<int> RwObjectService::getLineIndexes(std::vector<int>& internal, std::string& line)
{
  std::vector<std::string> indexes = this->split( line, ' ' );

  std::reverse( indexes.begin(), indexes.end() );
  indexes.pop_back();

  while(!indexes.empty())
  {
    internal.push_back( atoi( indexes.back().c_str() ) );
    indexes.pop_back();
  }

  LOG( 1, "internal: %s", internal[0] );
  LOG( 1, "internal.size: %s", internal.size() );
  return internal;
}

std::vector<Coordinate*> RwObjectService::getVertexes(std::vector<int>& indexes, std::vector<big_double>& coordinates)
{
  big_double x, y, z;
  Coordinate* coordinate;

  std::vector<Coordinate*> internal;
  std::reverse( indexes.begin(), indexes.end() );

  while(!indexes.empty())
  {
    x = coordinates[indexes.back()]; indexes.pop_back();
    y = coordinates[indexes.back()]; indexes.pop_back();
    z = coordinates[indexes.back()]; indexes.pop_back();

    coordinate = new Coordinate( x, y, z );
    internal.push_back( coordinate );
  }

  LOG( 1, "internal: %s", internal[0] );
  LOG( 1, "internal.size: %s", internal.size() );
  return internal;
}

std::vector<std::string> RwObjectService::split(std::string& line, char delimiter)
{
  std::vector<std::string> internal;
  std::stringstream ss(line); // Turn the std::string into a stream.
  std::string tok;

  while(getline(ss, tok, delimiter)) {
    internal.push_back(tok);
  }

  LOG( 1, "internal: %s", internal[0] );
  LOG( 1, "internal.size: %s", internal.size() );
  return internal;
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
