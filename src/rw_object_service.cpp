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

  std::vector<Coordinate*> coordinates;
  std::string name;
  std::string cstype;

  _last_index = 0;
  std::ifstream in_file(file_path);

  if( in_file.is_open() ) {
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
      else if( line.front() == 'o' )
      {
        name = line.substr(2, line.length());
        LOG( 8, "name: %s, ", name );
      }
      else if( line.front() == 'w' )
      {
        if( name == "window" ) {
          std::vector<int> indexes;

          this->getLineIndexes( indexes, line );
          std::vector<Coordinate*> vertexes = this->getVertexes( indexes, coordinates );

          std::ostringstream contents;
          for( auto value : vertexes ) contents << *value << ", ";

          LOG( 8, "vertexes: %s, ", contents.str() );
          // throw std::runtime_error( contents.str() );
        }
        LOG( 8, "line: %s, ", line );
      }
      else if( line.size() > 6
          && line[0] == 'c' && line[1] == 's' && line[2] == 't' && line[3] == 'y' && line[4] == 'p' && line[5] == 'e' && line[6] == ' ' )
      {
        std::vector<std::string> contents = split(line, ' ');

        cstype = contents.back();
        LOG( 8, "cstype: %s, ", cstype );
      }
      else if( line.front() == 'v' && line[1] == ' ' ) {
        std::vector<std::string> indexes = split(line, ' ');

        if( indexes.size() > 2 ) {
          coordinates.push_back( new Coordinate( atof( indexes[1].c_str() ), atof( indexes[2].c_str() ), atof( indexes[3].c_str() ) ) );
        }
        else {
          coordinates.push_back( new Coordinate( atof( indexes[1].c_str() ), atof( indexes[2].c_str() ), 1.0 ) );
        }
      }
      else if( line.front() == 'p' )
      {
        std::vector<int> indexes;
        this->getLineIndexes( indexes, line );

        LOGL( 8, "indexes.size: %s, ", indexes.size() );
        for( auto value : indexes ) LOGLN( 8, "%s, ", value ); LOGLN( 8, "\n" );

        std::vector<Coordinate*> vertexes = this->getVertexes( indexes, coordinates );
        this->facade.addPoint( name, vertexes[0]->x, vertexes[0]->y, vertexes[0]->z );
      }
      else if( line.front() == 'f' )
      {
        std::vector<int> segment_list;
        std::vector<int> facets_count;
        this->getFacetIndexes( segment_list, facets_count, line );

        do
        {
          is_there_new_lines = static_cast<bool>( getline( myfile, line ) );
          is_to_skip_new_line_fetch = true;

          is_there_a_next_line = line.front() == 'f';
          if( !is_there_a_next_line ) break;

          this->getFacetIndexes( segment_list, facets_count, line );
          if( !is_there_new_lines ) break;
        }
        while( true );

        LOGL( 8, "segment_list.size: %s, ", segment_list.size() );
        for( auto value : segment_list ) LOGLN( 8, "%s, ", value ); LOGLN( 8, "\n" );

        std::vector<Coordinate*> vertexes = this->getFacetVertexes( segment_list, facets_count, coordinates );

        this->facade.addPolyhedron( name, vertexes, segment_list, facets_count,
            _origin_coordinate_value, _origin_coordinate_value);
      }
      else if( line.front() == 'l' )
      {
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

        std::vector<Coordinate*> vertexes = this->getVertexes( indexes, coordinates );

        if( vertexes.size() < 3 )
        {
          this->facade.addLine( name, vertexes[0]->x, vertexes[0]->y, vertexes[0]->z,
              vertexes[1]->x, vertexes[1]->y , vertexes[1]->z );
        }
        else
        {
          if( cstype.size() )
          {
            if( cstype == "bezier" || cstype == "bspline" )
            {
              this->facade.addPolygon( name, vertexes,
                  _origin_coordinate_value,
                  _origin_coordinate_value, cstype == "bezier" ? CurveType::BEZIER : CurveType::BSPLINE );
            }
          }
          else
          {
            this->facade.addPolygon( name, vertexes );
          }
        }

        cstype = "";
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

std::vector<Coordinate*> RwObjectService::getVertexes(std::vector<int>& indexes, std::vector<Coordinate*>& coordinates)
{
  LOG( 8, "..." );
  int index;
  int offset = 0;

  LOG( 8, "_last_index: %s", _last_index );
  if( _last_index && indexes.front() == 1 ) offset = _last_index;

  _last_index = coordinates.size();
  LOG( 8, "_last_index: %s, offset: %s", _last_index, offset );

  std::vector<Coordinate*> internal;
  std::reverse( indexes.begin(), indexes.end() );

  while(!indexes.empty())
  {
    index = indexes.back() - 1 + offset;
    indexes.pop_back();

    LOGL( 8, "Reading index %s, ", index );
    LOGLN( 8, "value %s", *coordinates[index] );
    LOGLN( 8, "\n" );

    internal.push_back( coordinates[index] );
  }

  LOGL( 8, "internal.size: %s, ", internal.size() );
  for( auto value : internal ) LOGLN( 8, "%s, ", *value );

  LOGLN( 8, "\n" );
  return internal;
}

void RwObjectService::getFacetIndexes(std::vector<int>& result, std::vector<int>& facets_count, std::string& line)
{
  LOG( 8, "..." );
  int facets_size;

  std::vector<std::string> indexes = this->split( line, ' ' );
  std::reverse( indexes.begin(), indexes.end() );

  indexes.pop_back();
  int last_facets_size = result.size();

  while(!indexes.empty())
  {
    big_double last = atoi( indexes.back().c_str() );
    indexes.pop_back();

    std::regex search_regex( "^\\d+" );
    std::sregex_iterator next(line.begin(), line.end(), search_regex);
    std::sregex_iterator end;

    if( next != end )
    {
      std::smatch match = *next;
      LOG( 8, "match: %s", match.str() );

      next++;
      last = atoi( match.str().c_str() );
    }

    result.push_back( last );
    LOG( 8, "last: %s, result.size: %s", last, result.size() );
  }

  facets_size = result.size() - last_facets_size;

  if( facets_size < 3 )
  {
    std::ostringstream contents;
    for( auto value : result ) contents << value << ", ";

    std::string error = tfm::format(
        "The facets size should have at least 3 sides, not %s sides: \n%s",
        facets_size, contents.str() );

    LOG( 1, "%s", error );
    throw std::runtime_error( error );
  }

  facets_count.push_back( facets_size );
}

std::vector<Coordinate*> RwObjectService::getFacetVertexes(std::vector<int>& line_segments,
    std::vector<int>& facets_count, std::vector<Coordinate*>& coordinates)
{
  LOG( 8, "..." );
  int coordinates_size = coordinates.size();

  LOG( 8, "_last_index: %s", _last_index );
  LOG( 8, "coordinates_size: %s", coordinates_size );

  std::vector<Coordinate*> internal( coordinates.begin() + _last_index, coordinates.begin() + coordinates.size() );

  LOGL( 8, "internal.size: %s, ", internal.size() );
  for( auto value : internal ) LOGLN( 8, "%s, ", *value );
  LOGLN( 8, "\n" );

  for( auto& element : line_segments ) {
    element = element - _last_index;
  }

  LOGL( 8, "line_segments.size: %s, ", line_segments.size() );
  for( auto value : line_segments ) LOGLN( 8, "%s, ", value );
  LOGLN( 8, "\n" );

  _last_index = coordinates.size();
  LOG( 8, "_last_index: %s", _last_index );

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
  auto polyhedrons = this->facade._world._polyhedrons.getObjects();

  unsigned int index = 1;
  unsigned int last_index;

  LOG(8, "Write General Polygons");
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

  LOG(8, "Write General Polyhedrons");
  for (auto object : polyhedrons)
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

    myfile << "vn 0.0 0.0 1.0";
    auto line_segments = object->lineSegments();
    auto facets_count = object->facetsCount();
    auto facets_count_end = facets_count.end();
    auto facets_count_iterator = facets_count.begin();
    int facets_count_now = *facets_count_iterator;

    myfile << "\nf ";

    for( auto segment : line_segments )
    {
      if( facets_count_now == 0 ) {
        ++facets_count_iterator;

        if( facets_count_iterator == facets_count_end ) {
          auto error = tfm::format("The polyhedron has not enough facets count: \n%s", *object);

          LOG( 1, "%s", error );
          throw std::runtime_error(error);
        }

        facets_count_now = *facets_count_iterator;
        myfile << "\nf ";
      }

      --facets_count_now;
      myfile << std::to_string(index + segment - 1) + "//1 ";
    }

    index = index + objectCoordinates.size();
    myfile << "\n\n";
  }

  LOG(8, "Write Curves Types");
  for( auto object : curves )
  {
    LOG(8, "curve: %s", *object);
    if( !object->isVisibleOnGui() ) continue;

    myfile << "o " + object->getName();
    myfile << "\n";
    auto objectCoordinates = object->worldCoordinates();

    // https://stackoverflow.com/questions/351845/finding-the-type-of-an-object-in-c
    myfile << "cstype ";
    myfile << ( dynamic_cast<BezierCurve*> (object) ? "bezier" : "bspline" );
    myfile << "\n";

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

  LOG(8, "Write Lines Types");
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

  LOG(8, "Write Points Types");
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
  std::vector<Coordinate*> coordinates{new Coordinate(1, 2, 3), new Coordinate(4, 5, 6)};
  std::vector<Coordinate*> results = rw_object_service.getVertexes(indexes, coordinates);

  std::ostringstream contents;
  for( auto value : results ) contents << *value << ", ";
  CHECK( "(1, 2, 3, 1), (4, 5, 6, 1), " == contents.str() );
}


TEST_CASE("Testing basic getVertexes case load re-indexing")
{
  // _debugger_int_debug_level = 127-16;
  // https://stackoverflow.com/questions/9055778/initializing-a-reference-to-member-to-null-in-c
  Facade& nullface( *static_cast<Facade*>( nullptr ) );
  RwObjectService rw_object_service( nullface );

  std::vector<int> indexes{1, 2};
  std::vector<Coordinate*> coordinates{new Coordinate(1, 2, 3), new Coordinate(4, 5, 6)};
  std::vector<Coordinate*> results = rw_object_service.getVertexes(indexes, coordinates);

  std::vector<int> indexes2{3, 4};
  std::vector<Coordinate*> coordinates2{new Coordinate(1, 2, 3), new Coordinate(4, 5, 6),
      new Coordinate(7, 8, 9), new Coordinate(10, 11, 12)};
  std::vector<Coordinate*> results2 = rw_object_service.getVertexes(indexes2, coordinates2);

  std::ostringstream contents;
  for( auto value : results2 ) contents << *value << ", ";
  CHECK( "(7, 8, 9, 1), (10, 11, 12, 1), " == contents.str() );
}


TEST_CASE("Testing basic getVertexes case load after re-indexing")
{
  // _debugger_int_debug_level = 127-16;
  // https://stackoverflow.com/questions/9055778/initializing-a-reference-to-member-to-null-in-c
  Facade& nullface( *static_cast<Facade*>( nullptr ) );
  RwObjectService rw_object_service( nullface );

  std::vector<int> indexes{1, 2};
  std::vector<Coordinate*> coordinates{new Coordinate(1, 2, 3), new Coordinate(4, 5, 6)};
  std::vector<Coordinate*> results = rw_object_service.getVertexes(indexes, coordinates);

  std::vector<int> indexes2{3, 4};
  std::vector<Coordinate*> coordinates2{new Coordinate(1, 2, 3), new Coordinate(4, 5, 6),
      new Coordinate(7, 8, 9), new Coordinate(10, 11, 12)};
  std::vector<Coordinate*> results2 = rw_object_service.getVertexes(indexes2, coordinates2);

  std::vector<int> indexes3{5, 6};
  std::vector<Coordinate*> coordinates3{new Coordinate(1, 2, 3), new Coordinate(4, 5, 6),
      new Coordinate(7, 8, 9), new Coordinate(10, 11, 12), new Coordinate(13, 14, 15), new Coordinate(16, 17, 18)};
  std::vector<Coordinate*> results3 = rw_object_service.getVertexes(indexes3, coordinates3);

  std::ostringstream contents;
  for( auto value : results3 ) contents << *value << ", ";
  CHECK( "(13, 14, 15, 1), (16, 17, 18, 1), " == contents.str() );
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


TEST_CASE("Testing basic getFacetIndexes case load with 3 facets_size")
{
  // _debugger_int_debug_level = 127-16;
  // https://stackoverflow.com/questions/9055778/initializing-a-reference-to-member-to-null-in-c
  Facade& nullface( *static_cast<Facade*>( nullptr ) );
  RwObjectService rw_object_service( nullface );

  std::vector<int> indexes{};
  std::vector<int> facets_count{};
  std::string line = "f 1//1 2//1 4//1";
  rw_object_service.getFacetIndexes(indexes, facets_count, line);

  std::ostringstream contents;
  for( auto value : indexes ) contents << value << ", ";
  CHECK( "1, 2, 4, " == contents.str() );

  line = "f 3//2 4//2 8//2";
  rw_object_service.getFacetIndexes(indexes, facets_count, line);

  std::ostringstream contents2;
  for( auto value : indexes ) contents2 << value << ", ";
  CHECK( "1, 2, 4, 3, 4, 8, " == contents2.str() );

  std::ostringstream contents1;
  for( auto value : facets_count ) contents1 << value << ", ";
  CHECK( "3, 3, " == contents1.str() );
}


TEST_CASE("Testing basic getFacetIndexes case load with 4 facets_size")
{
  // _debugger_int_debug_level = 127-16;
  // https://stackoverflow.com/questions/9055778/initializing-a-reference-to-member-to-null-in-c
  Facade& nullface( *static_cast<Facade*>( nullptr ) );
  RwObjectService rw_object_service( nullface );

  std::vector<int> indexes{};
  std::vector<int> facets_count{};
  std::string line = "f 1//1 2//1 4//1 3//1";
  rw_object_service.getFacetIndexes(indexes, facets_count, line);

  std::ostringstream contents;
  for( auto value : indexes ) contents << value << ", ";
  CHECK( "1, 2, 4, 3, " == contents.str() );

  line = "f 3//2 4//2 8//2 7//2";
  rw_object_service.getFacetIndexes(indexes, facets_count, line);

  std::ostringstream contents2;
  for( auto value : indexes ) contents2 << value << ", ";
  CHECK( "1, 2, 4, 3, 3, 4, 8, 7, " == contents2.str() );

  std::ostringstream contents1;
  for( auto value : facets_count ) contents1 << value << ", ";
  CHECK( "4, 4, " == contents1.str() );
}


TEST_CASE("Testing invalid getFacetIndexes case load with 2 facets")
{
  // _debugger_int_debug_level = 127-16;
  // https://stackoverflow.com/questions/9055778/initializing-a-reference-to-member-to-null-in-c
  Facade& nullface( *static_cast<Facade*>( nullptr ) );
  RwObjectService rw_object_service( nullface );

  std::vector<int> indexes{};
  std::vector<int> facets_count{};
  std::string line = "f 1//1 2//1";

  // https://github.com/onqtam/doctest/blob/master/doc/markdown/assertions.md
  REQUIRE_THROWS_AS( rw_object_service.getFacetIndexes(indexes, facets_count, line), std::runtime_error );
}


TEST_CASE("Testing getFacetIndexes case load with 5 facets")
{
  // _debugger_int_debug_level = 127-16;
  // https://stackoverflow.com/questions/9055778/initializing-a-reference-to-member-to-null-in-c
  Facade& nullface( *static_cast<Facade*>( nullptr ) );
  RwObjectService rw_object_service( nullface );

  std::vector<int> indexes{};
  std::vector<int> facets_count{};
  std::string line = "f 3//2 4//2 8//2 7//2 2//1";

  // https://github.com/onqtam/doctest/blob/master/doc/markdown/assertions.md
  // REQUIRE_THROWS_AS( rw_object_service.getFacetIndexes(indexes, facets_count, line), std::runtime_error );
  rw_object_service.getFacetIndexes(indexes, facets_count, line);

  std::ostringstream contents;
  for( auto value : indexes ) contents << value << ", ";
  CHECK( "3, 4, 8, 7, 2, " == contents.str() );

  std::ostringstream contents1;
  for( auto value : facets_count ) contents1 << value << ", ";
  CHECK( "5, " == contents1.str() );
}


TEST_CASE("Testing getFacetIndexes case load with 7 facets")
{
  // _debugger_int_debug_level = 127-16;
  // https://stackoverflow.com/questions/9055778/initializing-a-reference-to-member-to-null-in-c
  Facade& nullface( *static_cast<Facade*>( nullptr ) );
  RwObjectService rw_object_service( nullface );

  std::vector<int> indexes{};
  std::vector<int> facets_count{};
  std::string line = "f 3//2 4//2 8//2 7//2";
  rw_object_service.getFacetIndexes(indexes, facets_count, line);

  line = "f 3//2 4//2 8//2";
  rw_object_service.getFacetIndexes(indexes, facets_count, line);
  // https://github.com/onqtam/doctest/blob/master/doc/markdown/assertions.md
  // REQUIRE_THROWS_AS( rw_object_service.getFacetIndexes(indexes, facets_count, line), std::runtime_error );

  std::ostringstream contents;
  for( auto value : indexes ) contents << value << ", ";
  CHECK( "3, 4, 8, 7, 3, 4, 8, " == contents.str() );

  std::ostringstream contents1;
  for( auto value : facets_count ) contents1 << value << ", ";
  CHECK( "4, 3, " == contents1.str() );
}


TEST_CASE("Testing basic getFacetVertexes case load with load count recovery")
{
  // _debugger_int_debug_level = 127-16;
  // https://stackoverflow.com/questions/9055778/initializing-a-reference-to-member-to-null-in-c
  Facade& nullface( *static_cast<Facade*>( nullptr ) );
  RwObjectService rw_object_service( nullface );

  std::vector<int> indexes{1, 2};
  std::vector<int> facets_count{};
  std::vector<Coordinate*> coordinates{new Coordinate(1, 2, 3), new Coordinate(4, 5, 6)};
  std::vector<Coordinate*> results = rw_object_service.getFacetVertexes(indexes, facets_count, coordinates);

  std::ostringstream contents;
  for( auto value : results ) contents << *value << ", ";
  CHECK( "(1, 2, 3, 1), (4, 5, 6, 1), " == contents.str() );

  std::ostringstream contents2;
  for( auto value : indexes ) contents2 << value << ", ";
  CHECK( "1, 2, " == contents2.str() );

  std::vector<int> indexes2{3, 4};
  std::vector<Coordinate*> coordinates2{new Coordinate(1, 2, 3), new Coordinate(4, 5, 6),
      new Coordinate(7, 8, 9), new Coordinate(10, 11, 12)};
  std::vector<Coordinate*> results2 = rw_object_service.getFacetVertexes(indexes2, facets_count, coordinates2);

  std::ostringstream contents3;
  for( auto value : results2 ) contents3 << *value << ", ";
  CHECK( "(7, 8, 9, 1), (10, 11, 12, 1), " == contents3.str() );

  std::ostringstream contents4;
  for( auto value : indexes2 ) contents4 << value << ", ";
  CHECK( "1, 2, " == contents4.str() );

  std::ostringstream contents1;
  for( auto value : facets_count ) contents1 << value << ", ";
  CHECK( "" == contents1.str() );
}

