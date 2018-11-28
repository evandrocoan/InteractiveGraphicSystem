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
  std::vector<Coordinate> input;

  for( auto coordinate : this->_windowCoordinates ) {
    input.push_back(Coordinate(*coordinate));
  }

  std::vector<Coordinate> temporary;
  std::vector<Coordinate> clippingResult;

  this->_sutherlandHodgmanClipLeft(axes, input, temporary);
  this->_sutherlandHodgmanClipRight(axes, temporary, clippingResult);
  this->_sutherlandHodgmanClipTop(axes, clippingResult, temporary);
  this->_sutherlandHodgmanClipBottom(axes, temporary, clippingResult);

  if( clippingResult.size() == 0 ) {
    return false;
  }

  this->destroyList(this->_clippingCoordinates);

  for( auto coordinate : clippingResult ) {
    this->_clippingCoordinates.push_back(new Coordinate(coordinate));
  }
  return true;
}

void Polygon::_sutherlandHodgmanClipLeft(const Axes& axes, std::vector<Coordinate>& input, std::vector<Coordinate>& clippingResult)
{
  if( clippingResult.size() > 0 ) {
    clippingResult.clear();
  }

  if( input.size() == 0 ) {
    return;
  }

  big_double clipX = axes.xWiMin;
  input.push_back(input[0]);

  for( unsigned int i = 0; i < input.size() - 1; i++ )
  {
    Coordinate c0 = input[i];
    Coordinate c1 = input[i+1];

    // Case 1: out -> out
    if( c0.x < clipX && c1.x < clipX ) {
    }

    // Case 2: in -> in
    if( c0.x >= clipX && c1.x >= clipX )
    {
      clippingResult.push_back(c1);
    }

    big_double x = clipX;
    big_double m = (c1.y-c0.y) / (c1.x-c0.x);
    big_double y = m * (x-c0.x) + c0.y;

    // Case 3: in -> out
    if( c0.x >= clipX && c1.x < clipX )
    {
      clippingResult.emplace_back(x,y,0);
    }

    // Case 4: out -> in
    if( c0.x < clipX && c1.x >= clipX )
    {
      clippingResult.emplace_back(x,y,0);
      clippingResult.push_back(c1);
    }
  }
}

void Polygon::_sutherlandHodgmanClipRight(const Axes& axes, std::vector<Coordinate>& input, std::vector<Coordinate>& clippingResult)
{
  if( clippingResult.size() > 0 ) {
    clippingResult.clear();
  }

  if( input.size() == 0 ) {
    return;
  }

  big_double clipX = axes.xWiMax;
  input.push_back(input[0]);

  for( unsigned int i = 0; i < input.size()-1; i++ )
  {
    Coordinate c0 = input[i];
    Coordinate c1 = input[i+1];

    // Case 1: out -> out
    if( c0.x >= clipX && c1.x >= clipX ) {
    }

    // Case 2: in -> in
    if( c0.x < clipX && c1.x < clipX )
    {
      clippingResult.push_back(c1);
    }

    big_double x = clipX;
    big_double m = (c1.y-c0.y) / (c1.x-c0.x);
    big_double y = m * (x-c0.x) + c0.y;

    // Case 3: in -> out
    if( c0.x < clipX && c1.x >= clipX )
    {
      clippingResult.emplace_back(x,y,0);
    }

    // Case 4: out -> in
    if( c0.x >= clipX && c1.x < clipX )
    {
      clippingResult.emplace_back(x,y,0);
      clippingResult.push_back(c1);
    }
  }
}

void Polygon::_sutherlandHodgmanClipTop(const Axes& axes, std::vector<Coordinate>& input, std::vector<Coordinate>& clippingResult)
{
  if( clippingResult.size() > 0 ) {
    clippingResult.clear();
  }

  if( input.size() == 0 ) {
    return;
  }

  big_double clipY = axes.yWiMax;
  input.push_back(input[0]);

  for( unsigned int i = 0; i < input.size()-1; i++ )
  {
    Coordinate c0 = input[i];
    Coordinate c1 = input[i+1];

    // Case 1: out -> out
    if( c0.y > clipY && c1.y > clipY ) {
    }

    // Case 2: in -> in
    if( c0.y <= clipY && c1.y <= clipY )
    {
      clippingResult.push_back(c1);
    }

    big_double y = clipY;
    big_double m = (c1.x-c0.x) / (c1.y-c0.y);
    big_double x = m * (y-c0.y) + c0.x;

    // Case 3: in -> out
    if( c0.y <= clipY && c1.y > clipY )
    {
      clippingResult.emplace_back(x,y,0);
    }

    // Case 4: out -> in
    if( c0.y > clipY && c1.y <= clipY )
    {
      clippingResult.emplace_back(x,y,0);
      clippingResult.push_back(c1);
    }
  }
}

void Polygon::_sutherlandHodgmanClipBottom(const Axes& axes, std::vector<Coordinate>& input, std::vector<Coordinate>& clippingResult)
{
  if( clippingResult.size() > 0 ) {
    clippingResult.clear();
  }

  if( input.size() == 0 ) {
    return;
  }

  big_double clipY = axes.yWiMin;
  input.push_back(input[0]);

  for( unsigned int i = 0; i < input.size()-1; i++ )
  {
    Coordinate c0 = input[i];
    Coordinate c1 = input[i+1];

    // Case 1: out -> out
    if( c0.y < clipY && c1.y < clipY ) {
    }

    // Case 2: in -> in
    if( c0.y >= clipY && c1.y >= clipY ) {
      clippingResult.push_back(c1);
    }

    big_double y = clipY;
    big_double m = (c1.x-c0.x) / (c1.y-c0.y);
    big_double x = m * (y-c0.y) + c0.x;

    // Case 3: in -> out
    if( c0.y >= clipY && c1.y < clipY )
    {
      clippingResult.emplace_back(x,y,0);
    }

    // Case 4: out -> in
    if( c0.y < clipY && c1.y >= clipY )
    {
      clippingResult.emplace_back(x,y,0);
      clippingResult.push_back(c1);
    }
  }
}
