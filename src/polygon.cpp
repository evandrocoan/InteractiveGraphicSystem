#include "polygon.h"

Polygon::Polygon(std::string name, std::vector<Coordinate*> _worldCoordinates,
            Coordinate _borderColor, Coordinate _fillingColor, bool _isVisibleOnGui) :
      DrawableObject(name, _worldCoordinates, _borderColor, _fillingColor, _isVisibleOnGui)
{
}

Polygon::~Polygon()
{
}

void Polygon::updateClippingCoordinates(const Axes& axes)
{
  LOG( 8, "..." );
  this->_isDrawable = this->_sutherlandHodgmanPolygonClip(axes);
}

bool Polygon::_sutherlandHodgmanPolygonClip(const Axes& axes)
{
  std::vector<Coordinate*> input;

  for( auto coordinate : this->_windowCoordinates ) {
    input.push_back(new Coordinate(*coordinate));
  }

  std::vector<Coordinate*> temporary;
  std::vector<Coordinate*> clippingResult;

  this->_sutherlandHodgmanClipLeft(axes, input, temporary);
  this->_sutherlandHodgmanClipRight(axes, temporary, clippingResult);
  this->_sutherlandHodgmanClipTop(axes, clippingResult, temporary);
  this->_sutherlandHodgmanClipBottom(axes, temporary, clippingResult);

  if( clippingResult.size() == 0 ) {
    return false;
  }

  this->destroyList(this->_clippingCoordinates);

  for( auto coordinate : clippingResult ) {
    this->_clippingCoordinates.push_back( new Coordinate(*coordinate) );
  }
  return true;
}

void Polygon::_sutherlandHodgmanClipLeft(const Axes& axes, std::vector<Coordinate*>& input, std::vector<Coordinate*>& clippingResult)
{
  if( clippingResult.size() > 0 ) {
    clippingResult.clear();
  }

  if( input.size() == 0 ) {
    return;
  }

  input.push_back(input[0]);

  for( unsigned int i = 0; i < input.size() - 1; i++ )
  {
    Coordinate* first_coordinate = input[i];
    Coordinate* second_coordinate = input[i+1];

    // Case 1: out -> out
    if( first_coordinate->x < axes.xWiMin && second_coordinate->x < axes.xWiMin ) {
    }

    // Case 2: in -> in
    if( first_coordinate->x >= axes.xWiMin && second_coordinate->x >= axes.xWiMin ) {
      clippingResult.push_back( second_coordinate );
    }

    big_double new_x_value = axes.xWiMin;
    big_double inclination = (second_coordinate->y-first_coordinate->y) / (second_coordinate->x-first_coordinate->x);
    big_double new_y_value = inclination * (new_x_value-first_coordinate->x) + first_coordinate->y;

    // Case 3: in -> out
    if( first_coordinate->x >= axes.xWiMin && second_coordinate->x < axes.xWiMin ) {
      clippingResult.push_back( new Coordinate(new_x_value, new_y_value, 0) );
    }

    // Case 4: out -> in
    if( first_coordinate->x < axes.xWiMin && second_coordinate->x >= axes.xWiMin )
    {
      clippingResult.push_back( new Coordinate(new_x_value, new_y_value, 0) );
      clippingResult.push_back( second_coordinate );
    }
  }
}

void Polygon::_sutherlandHodgmanClipRight(const Axes& axes, std::vector<Coordinate*>& input, std::vector<Coordinate*>& clippingResult)
{
  if( clippingResult.size() > 0 ) {
    clippingResult.clear();
  }

  if( input.size() == 0 ) {
    return;
  }

  input.push_back(input[0]);

  for( unsigned int i = 0; i < input.size()-1; i++ )
  {
    Coordinate* first_coordinate = input[i];
    Coordinate* second_coordinate = input[i+1];

    // Case 1: out -> out
    if( first_coordinate->x >= axes.xWiMax && second_coordinate->x >= axes.xWiMax ) {
    }

    // Case 2: in -> in
    if( first_coordinate->x < axes.xWiMax && second_coordinate->x < axes.xWiMax ) {
      clippingResult.push_back( second_coordinate );
    }

    big_double new_x_value = axes.xWiMax;
    big_double inclination = (second_coordinate->y-first_coordinate->y) / (second_coordinate->x-first_coordinate->x);
    big_double new_y_value = inclination * (new_x_value-first_coordinate->x) + first_coordinate->y;

    // Case 3: in -> out
    if( first_coordinate->x < axes.xWiMax && second_coordinate->x >= axes.xWiMax ) {
      clippingResult.push_back( new Coordinate(new_x_value, new_y_value, 0) );
    }

    // Case 4: out -> in
    if( first_coordinate->x >= axes.xWiMax && second_coordinate->x < axes.xWiMax )
    {
      clippingResult.push_back( new Coordinate(new_x_value, new_y_value, 0) );
      clippingResult.push_back( second_coordinate );
    }
  }
}

void Polygon::_sutherlandHodgmanClipTop(const Axes& axes, std::vector<Coordinate*>& input, std::vector<Coordinate*>& clippingResult)
{
  if( clippingResult.size() > 0 ) {
    clippingResult.clear();
  }

  if( input.size() == 0 ) {
    return;
  }

  input.push_back(input[0]);

  for( unsigned int i = 0; i < input.size()-1; i++ )
  {
    Coordinate* first_coordinate = input[i];
    Coordinate* second_coordinate = input[i+1];

    // Case 1: out -> out
    if( first_coordinate->y > axes.yWiMax && second_coordinate->y > axes.yWiMax ) {
    }

    // Case 2: in -> in
    if( first_coordinate->y <= axes.yWiMax && second_coordinate->y <= axes.yWiMax ) {
      clippingResult.push_back( second_coordinate );
    }

    big_double new_y_value = axes.yWiMax;
    big_double inclination = (second_coordinate->x-first_coordinate->x) / (second_coordinate->y-first_coordinate->y);
    big_double new_x_value = inclination * (new_y_value-first_coordinate->y) + first_coordinate->x;

    // Case 3: in -> out
    if( first_coordinate->y <= axes.yWiMax && second_coordinate->y > axes.yWiMax ) {
      clippingResult.push_back( new Coordinate(new_x_value, new_y_value, 0) );
    }

    // Case 4: out -> in
    if( first_coordinate->y > axes.yWiMax && second_coordinate->y <= axes.yWiMax )
    {
      clippingResult.push_back( new Coordinate(new_x_value, new_y_value, 0) );
      clippingResult.push_back( second_coordinate );
    }
  }
}

void Polygon::_sutherlandHodgmanClipBottom(const Axes& axes, std::vector<Coordinate*>& input, std::vector<Coordinate*>& clippingResult)
{
  if( clippingResult.size() > 0 ) {
    clippingResult.clear();
  }

  if( input.size() == 0 ) {
    return;
  }

  input.push_back(input[0]);

  for( unsigned int i = 0; i < input.size()-1; i++ )
  {
    Coordinate* first_coordinate = input[i];
    Coordinate* second_coordinate = input[i+1];

    // Case 1: out -> out
    if( first_coordinate->y < axes.yWiMin && second_coordinate->y < axes.yWiMin ) {
    }

    // Case 2: in -> in
    if( first_coordinate->y >= axes.yWiMin && second_coordinate->y >= axes.yWiMin ) {
      clippingResult.push_back( second_coordinate );
    }

    big_double new_y_value = axes.yWiMin;
    big_double inclination = (second_coordinate->x-first_coordinate->x) / (second_coordinate->y-first_coordinate->y);
    big_double new_x_value = inclination * (axes.yWiMin-first_coordinate->y) + first_coordinate->x;

    // Case 3: in -> out
    if( first_coordinate->y >= axes.yWiMin && second_coordinate->y < axes.yWiMin ) {
      clippingResult.push_back( new Coordinate(new_x_value, new_y_value, 0) );
    }

    // Case 4: out -> in
    if( first_coordinate->y < axes.yWiMin && second_coordinate->y >= axes.yWiMin )
    {
      clippingResult.push_back( new Coordinate(new_x_value, new_y_value, 0) );
      clippingResult.push_back( second_coordinate );
    }
  }
}
