#include "polyhedron.h"
#include "polygon.h"

Polyhedron::Polyhedron(std::string name, std::vector<Coordinate*> _worldCoordinates,
              std::vector<unsigned int> _line_segments, int _facet_size,
              Coordinate _borderColor, Coordinate _fillingColor) :
      DrawableObject(name, _worldCoordinates, _borderColor, _fillingColor),
      _facet_size(_facet_size),
      _line_segments(_line_segments)
{
  // 4 facets (blender export)
  // f 1//1 2//1 4//1 3//1
  // f 3//2 4//2 8//2 7//2
  // f 7//3 8//3 6//3 5//3
  // ...
  //
  // versus
  //
  // 3 facets
  // f 1//2 7//2 5//2
  // f 1//2 3//2 7//2
  // f 1//6 4//6 3//6
  // ...
  if( _facet_size == 3 || _facet_size == 4 ) {
    int segments = _line_segments.size();

    if( segments % _facet_size != 0 )
    {
      std::string error = tfm::format(
          "Invalid Polyhedron facets and line segments paring, %s % %s = %s, but should be 0.",
          segments, _facet_size, segments % _facet_size );

      LOG( 1, "%s", error );
      throw std::runtime_error( error );
    }
  }
  else {
    std::string error = tfm::format(
        "A Polyhedron should have facets with 3 or 4 sides, not %s.", _facet_size );

    LOG( 1, "%s", error );
    throw std::runtime_error( error );
  }

  if( _worldCoordinates.size() < 4 )
  {
    std::ostringstream contents;
    for( auto value : _worldCoordinates ) contents << *value << ", ";

    contents << "[";
    for( auto value : _line_segments ) contents << value << ", ";
    contents << "]";

    std::string error = tfm::format(
        "A Polyhedron should have at least 4 coordinates or line segments, not %s.\n\n%s",
        _worldCoordinates.size(), contents.str() );

    LOG( 1, "%s", error );
    throw std::runtime_error( error );
  }
}

Polyhedron::~Polyhedron()
{
  for( auto polygon : this->_polygons )
  {
    delete polygon;
  }
}

const std::vector<Polygon*> Polyhedron::getPolygons() const {
  return this->_polygons;
}

void Polyhedron::updateClippingCoordinates(const Axes& axes)
{
  LOG( 8, "..." );
  for( auto polygon : this->_polygons )
  {
    polygon->updateClippingCoordinates(axes);
  }
}

// https://www.codeproject.com/Articles/25237/Bezier-Curves-Made-Simple
void Polyhedron::updateWindowCoordinates(const Transformation& transformation)
{
  LOG( 8, "..." );
  Polygon* polygon;

  auto coordinates = this->worldCoordinates();
  DrawableObject::destroyList(this->_polygons);

  for( int slice = _line_segments.size(); slice > 0; slice -= _facet_size )
  {
    LOG( 8, "slice: %s", slice );
    std::vector<unsigned int> facets( _line_segments.begin() + slice - _facet_size, _line_segments.begin() + slice );

    std::ostringstream contents1;
    for( auto value : facets ) contents1 << value << ", ";
    LOG( 8, "facets: %s", contents1.str() );

    std::vector<Coordinate*> sides;
    sides.reserve(_facet_size);

    for( auto index : facets ) {
      LOG( 8, "coordinate: %s", *coordinates[index-1] );
      sides.push_back( coordinates[index-1] );
    }

    std::ostringstream contents2;
    for( auto value : sides ) contents2 << *value << ", ";
    LOG( 8, "sides: %s", contents2.str() );

    polygon = new Polygon( this->getName() + "[" + std::to_string(slice) + "]", sides,
        _default_coordinate_value_parameter, _default_coordinate_value_parameter, false );

    LOG( 8, "polygon: %s", *polygon );
    polygon->updateWindowCoordinates( transformation );

    this->_polygons.push_back( polygon );
  }
}

