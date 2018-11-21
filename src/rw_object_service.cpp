/*
 * rw_object_service.cpp
 *
 *  Created on: 23 de mar de 2018
 *      Author: karla
 */

#include "rw_object_service.h"
#include "doctest_include.h"

RwObjectService::RwObjectService(Facade& facade) :
      facade(facade),
      _last_index(0)
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

  std::vector<big_double> coordinates_points;
  std::string name;

  _last_index = 0;
  std::ifstream in_file(file_path);

  if( in_file.is_open() )
  {
    std::stringstream myfile;

    // removes all comments and empty lines
    while( getline( in_file, line ) )
    {
      line = std::regex_replace( line, std::regex( "#.+" ), "" );
      line = std::regex_replace( line, std::regex( "( |\\t)+" ), " " );
      line = std::regex_replace( line, std::regex( "^( |\\t)+|( |\\t)+$" ), "" );

      if( line.size() > 1 )
      {
        myfile << line << '\n';
      }
    }

    while( true )
    {
      if( is_to_skip_new_line_fetch ) is_to_skip_new_line_fetch = false;
          else is_there_new_lines = static_cast<bool>( getline( myfile, line ) );

      if( !is_there_new_lines ) break;
      // LOG( 8, "line: %s", line );

      // https://www.fileformat.info/format/wavefrontobj/egff.htm
      if( line.front() == 'o' )
      {
        name = line.substr(2, line.length());
        LOG( 8, "name: %s, ", name );
      }
      else if( line.front() == 'v' )
      {
        std::vector<std::string> indexes = split(line, ' ');

        if( indexes.size() > 2 )
        {
          coordinates_points.push_back( atof( indexes[1].c_str() ) );
          coordinates_points.push_back( atof( indexes[2].c_str() ) );
          coordinates_points.push_back( atof( indexes[3].c_str() ) );
        }
        else
        {
          coordinates_points.push_back( atof( indexes[1].c_str() ) );
          coordinates_points.push_back( atof( indexes[2].c_str() ) );
          coordinates_points.push_back( 1.0 );
        }
      }
      else if( line.front() == 'p' )
      {
        std::vector<int> indexes;
        this->getLineIndexes( indexes, line );

        LOGL( 8, "indexes.size: %s, ", indexes.size() );
        for( auto value : indexes ) LOGLN( 8, "%s, ", value ); LOGLN( 8, "\n" );

        std::vector<Coordinate*> vertexes = this->getVertexes( indexes, coordinates_points );
        this->facade.addPoint( name, vertexes[0]->x, vertexes[0]->y, vertexes[0]->z );
      }
      else if( line.front() == 'l' || ( line.front() == 'b' && ( line[1] == 'z' || line[1] == 's' ) ) )
      {
        char line_front = line.front();
        char line_second = line[1];

        std::vector<int> indexes;
        this->getLineIndexes( indexes, line );

        do
        {
          is_there_new_lines = static_cast<bool>( getline( myfile, line ) );
          is_to_skip_new_line_fetch = true;

          is_there_a_next_line = line.front() == 'l';
          if( !is_there_a_next_line ) break;

          this->getLineIndexes( indexes, line );
          if( !is_there_new_lines ) break;
        }
        while( true );

        LOGL( 8, "indexes.size: %s, ", indexes.size() );
        for( auto value : indexes ) LOGLN( 8, "%s, ", value ); LOGLN( 8, "\n" );

        std::vector<Coordinate*> vertexes = this->getVertexes( indexes, coordinates_points );

        if( vertexes.size() < 3 )
        {
          this->facade.addLine( name, vertexes[0]->x, vertexes[0]->y, vertexes[0]->z,
              vertexes[1]->x, vertexes[1]->y , vertexes[1]->z );
        }
        else
        {
          if( line_front == 'b' )
          {
            this->facade.addPolygon( name, vertexes,
                _origin_coordinate_value,
                _origin_coordinate_value, line_second == 'z' ? CurveType::BEZIER : CurveType::BSPLINE );
          }
          else
          {
            this->facade.addPolygon( name, vertexes );
          }
        }
      }
    }

    in_file.close();
  }
}

void RwObjectService::getLineIndexes(std::vector<int>& result, std::string& line)
{
  LOG( 8, "..." );
  bool is_first = true;

  std::vector<std::string> indexes = this->split( line, ' ' );
  std::reverse( indexes.begin(), indexes.end() );

  indexes.pop_back();
  std::string front = indexes.back();

  while(!indexes.empty())
  {
    big_double last = atoi( indexes.back().c_str() );
    LOG( 8, "last: %s, result.size: %s", last, result.size() );

    if( !is_first || !result.size() || ( is_first && result.size() && result.back() != last ) )
    {
      result.push_back( last );
      LOG( 8, "pushing back: %s, result.size: %s", last, result.size() );
    }

    indexes.pop_back();
    is_first = false;
  }
}

std::vector<Coordinate*> RwObjectService::getVertexes(std::vector<int>& indexes, std::vector<big_double>& coordinates_points)
{
  LOG( 8, "..." );
  int index;

  int offset = 0;
  Coordinate* coordinate;

  LOG( 8, "_last_index: %s", _last_index );
  if( _last_index && indexes.front() == 1 ) offset = _last_index;

  _last_index = coordinates_points.size() / 3;
  LOG( 8, "_last_index: %s, offset: %s", _last_index, offset );

  std::vector<Coordinate*> internal;
  std::reverse( indexes.begin(), indexes.end() );

  while(!indexes.empty())
  {
    index = ( indexes.back() - 1 + offset ) * 3;
    indexes.pop_back();

    LOG( 8, "Reading index %s from %s", index, index / 3 );
    LOG( 8, "Reading x value %s", coordinates_points[index] );
    LOG( 8, "Reading y value %s", coordinates_points[index+1] );
    LOG( 8, "Reading z value %s", coordinates_points[index+2] );

    coordinate = new Coordinate( coordinates_points[index], coordinates_points[index+1], coordinates_points[index+2] );
    internal.push_back( coordinate );
  }

  LOGL( 8, "internal.size: %s, ", internal.size() );
  for( auto value : internal ) LOGLN( 8, "%s, ", *value );

  LOGLN( 8, "\n" );
  return internal;
}

std::vector<std::string> RwObjectService::split(std::string& line, char delimiter)
{
  LOG( 8, "..." );
  std::string tok;

  // Turn the std::string into a stream.
  std::stringstream ss(line);
  std::vector<std::string> internal;

  while( getline( ss, tok, delimiter ) ) {
    internal.push_back(tok);
  }

  LOGL( 8, "internal.size: %s, ", internal.size() );
  for( auto value : internal ) LOGLN( 8, "%s, ", value );

  LOGLN( 8, "\n" );
  return internal;
}

void RwObjectService::write(std::string file_path)
{
  LOG( 1, "Writing the displayFile objects..." );
  std::ofstream myfile;

  // https://stackoverflow.com/questions/1535922/c-change-newline-from-crlf-to-lf
  myfile.open(file_path, std::ios_base::binary | std::ios_base::out);

  myfile << "# Starting the file objects\n# "
      << PROGRAM_AUTHORS
      << "\n# https://github.com/evandrocoan/GraphicComputing"
      << "\n\n";

  auto lines = this->facade._world._lines.getObjects();
  auto points = this->facade._world._points.getObjects();
  auto curves = this->facade._world._curves.getObjects();
  auto polygons = this->facade._world._polygons.getObjects();

  unsigned int index = 1;
  unsigned int last_index;

  LOG(8, "Draw General Polygons");
  for (auto object : polygons)
  {
    LOG(8, "object: %s", *object);
    if( !object->isVisibleOnGui() ) continue;

    myfile << "o " + object->getName() + "\n";
    auto objectCoordinates = object->worldCoordinates();

    for( auto coordinate : objectCoordinates )
    {
      myfile << "v "
          << std::to_string( coordinate->x ) << " "
          << std::to_string( coordinate->y ) << " "
          << std::to_string( coordinate->z ) << "\n";
    }

    myfile << "l ";
    last_index = index + objectCoordinates.size();

    for( ; index < last_index ; index++ ) {
      myfile << std::to_string(index) + " ";
    }

    myfile << "\n\n";
  }

  LOG(8, "Draw Curves Types");
  for( auto object : curves )
  {
    LOG(8, "curve: %s", *object);
    if( !object->isVisibleOnGui() ) continue;

    myfile << "o " + object->getName() + "\n";
    auto objectCoordinates = object->worldCoordinates();

    for( auto coordinate : objectCoordinates )
    {
      myfile << "v "
          << std::to_string( coordinate->x ) << " "
          << std::to_string( coordinate->y ) << " "
          << std::to_string( coordinate->z ) << "\n";
    }

    // https://stackoverflow.com/questions/351845/finding-the-type-of-an-object-in-c
    myfile << ( dynamic_cast<BezierCurve*> (object) ? "bz " : "bs " );
    last_index = index + objectCoordinates.size();

    for( ; index < last_index ; index++ ) {
      myfile << std::to_string(index) + " ";
    }

    myfile << "\n\n";
  }

  LOG(8, "Draw Lines Types");
  for( auto object : lines )
  {
    LOG(8, "line: %s", *object);
    if( !object->isVisibleOnGui() ) continue;

    myfile << "o " + object->getName() + "\n";
    auto objectCoordinates = object->worldCoordinates();

    for( auto coordinate : objectCoordinates )
    {
      myfile << "v "
          << std::to_string( coordinate->x ) << " "
          << std::to_string( coordinate->y ) << " "
          << std::to_string( coordinate->z ) << "\n";
    }

    myfile << "l ";
    last_index = index + objectCoordinates.size();

    for( ; index < last_index ; index++ ) {
      myfile << std::to_string(index) + " ";
    }

    myfile << "\n\n";
  }

  LOG(8, "Draw Points Types");
  for( auto object : points )
  {
    LOG(8, "point: %s", *object);
    if( !object->isVisibleOnGui() ) continue;

    myfile << "o " + object->getName() + "\n";
    auto objectCoordinates = object->worldCoordinates();

    for( auto coordinate : objectCoordinates )
    {
      myfile << "v "
          << std::to_string( coordinate->x ) << " "
          << std::to_string( coordinate->y ) << " "
          << std::to_string( coordinate->z ) << "\n";
    }

    myfile << "p ";
    last_index = index + objectCoordinates.size();

    for( ; index < last_index ; index++ ) {
      myfile << std::to_string(index) + " ";
    }

    myfile << "\n\n";
  }

  myfile.close();
}


TEST_CASE("Testing basic getVertexes case load")
{
  // _debugger_int_debug_level = 127-16;
  // https://stackoverflow.com/questions/9055778/initializing-a-reference-to-member-to-null-in-c
  Facade& nullface( *static_cast<Facade*>( nullptr ) );
  RwObjectService rw_object_service( nullface );

  std::vector<int> indexes{1, 2};
  std::vector<big_double> coordinates_points{1, 2, 3, 4, 5, 6};
  std::vector<Coordinate*> results = rw_object_service.getVertexes(indexes, coordinates_points);

  std::ostringstream contents;
  for( auto value : results ) contents << *value << ", ";
  CHECK( "(1, 2, 3), (4, 5, 6), " == contents.str() );
}


TEST_CASE("Testing basic getVertexes case load re-indexing")
{
  // _debugger_int_debug_level = 127-16;
  // https://stackoverflow.com/questions/9055778/initializing-a-reference-to-member-to-null-in-c
  Facade& nullface( *static_cast<Facade*>( nullptr ) );
  RwObjectService rw_object_service( nullface );

  std::vector<int> indexes{1, 2};
  std::vector<big_double> coordinates_points{1, 2, 3, 4, 5, 6};
  std::vector<Coordinate*> results = rw_object_service.getVertexes(indexes, coordinates_points);

  std::vector<int> indexes2{1, 2};
  std::vector<big_double> coordinates_points2{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
  std::vector<Coordinate*> results2 = rw_object_service.getVertexes(indexes2, coordinates_points2);

  std::ostringstream contents;
  for( auto value : results2 ) contents << *value << ", ";
  CHECK( "(7, 8, 9), (10, 11, 12), " == contents.str() );
}


TEST_CASE("Testing basic getVertexes case load after re-indexing")
{
  // _debugger_int_debug_level = 127-16;
  // https://stackoverflow.com/questions/9055778/initializing-a-reference-to-member-to-null-in-c
  Facade& nullface( *static_cast<Facade*>( nullptr ) );
  RwObjectService rw_object_service( nullface );

  std::vector<int> indexes{1, 2};
  std::vector<big_double> coordinates_points{1, 2, 3, 4, 5, 6};
  std::vector<Coordinate*> results = rw_object_service.getVertexes(indexes, coordinates_points);

  std::vector<int> indexes2{1, 2};
  std::vector<big_double> coordinates_points2{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
  std::vector<Coordinate*> results2 = rw_object_service.getVertexes(indexes2, coordinates_points2);

  std::vector<int> indexes3{5, 6};
  std::vector<big_double> coordinates_points3{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18};
  std::vector<Coordinate*> results3 = rw_object_service.getVertexes(indexes3, coordinates_points3);

  std::ostringstream contents;
  for( auto value : results3 ) contents << *value << ", ";
  CHECK( "(13, 14, 15), (16, 17, 18), " == contents.str() );
}


TEST_CASE("Testing basic getLineIndexes loading duplicated vertexes")
{
  // _debugger_int_debug_level = 127-16;
  // https://stackoverflow.com/questions/9055778/initializing-a-reference-to-member-to-null-in-c
  Facade& nullface( *static_cast<Facade*>( nullptr ) );
  RwObjectService rw_object_service( nullface );

  std::vector<int> indexes{1, 2};
  std::string line = "l 2 3";
  rw_object_service.getLineIndexes(indexes, line);

  std::ostringstream contents;
  for( auto value : indexes ) contents << value << ", ";
  CHECK( "1, 2, 3, " == contents.str() );
}


TEST_CASE("Testing basic getLineIndexes loading duplicated vertexes on middle of line")
{
  // _debugger_int_debug_level = 127-16;
  // https://stackoverflow.com/questions/9055778/initializing-a-reference-to-member-to-null-in-c
  Facade& nullface( *static_cast<Facade*>( nullptr ) );
  RwObjectService rw_object_service( nullface );

  std::vector<int> indexes{1, 2};
  std::string line = "l 2 3 3 3 4";
  rw_object_service.getLineIndexes(indexes, line);

  std::ostringstream contents;
  for( auto value : indexes ) contents << value << ", ";
  CHECK( "1, 2, 3, 3, 3, 4, " == contents.str() );
}

