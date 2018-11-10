#include "line.h"

Line::Line(std::string name, Coordinate* line_cord1, Coordinate* line_cord2,
            Coordinate _borderColor, LineClippingType type, bool _visibleOnGUI) :
      DrawableObject(name, std::vector<Coordinate*>{line_cord1, line_cord2}, _borderColor, _visibleOnGUI),
      line_clipping_type(type)
{
}

Line::~Line()
{
}

inline std::ostream& operator<<(std::ostream &output, const LineClippingType object)
{
  switch( object )
  {
    case LineClippingType::LIANG_BARSKY:    output << "LIANG_BARSKY"; break;
    case LineClippingType::COHEN_SUTHELAND: output << "COHEN_SUTHELAND"; break;
    default:
      output << (int) object; break;
  }
  return output;
}

inline std::ostream& operator<<(std::ostream &output, const CohenSuthelandRegionCode object)
{
  switch( object )
  {
    case CohenSuthelandRegionCode::INSIDE: output << "INSIDE"; break;
    case CohenSuthelandRegionCode::LEFT:   output << "LEFT"; break;
    case CohenSuthelandRegionCode::RIGHT:  output << "RIGHT"; break;
    case CohenSuthelandRegionCode::BOTTOM: output << "BOTTOM"; break;
    case CohenSuthelandRegionCode::TOP:    output << "TOP"; break;
    default:
      output << (int) object; break;
  }
  return output;
}

void Line::updateClippingCoordinates(const Axes& axes)
{
  LOG (8, "..." );
  switch( this->line_clipping_type )
  {
    case LineClippingType::LIANG_BARSKY:    this->_isDrawable = this->_liangBaskyLineClip(axes); break;
    case LineClippingType::COHEN_SUTHELAND: this->_isDrawable = this->_cohenSuthelandLineClip(axes); break;
  }
  // LOG(8, "Line clipping update... %s %s %s", this->line_clipping_type, *this, axes);
}

bool Line::_liangBaskyLineClip(const Axes& axes)
{
  this->destroyList(this->_clippingCoordinates);

  for( auto coordinate : this->_windowCoordinates )
  {
    this->_clippingCoordinates.push_back(new Coordinate(*coordinate));
  }
  auto front = this->_clippingCoordinates.begin();

  auto& c1 = **front; ++front;
  auto& c2 = **front;

  if( c1 == c2 )
  {
    LOG(1, "" );
    LOG(1, "" );
    LOG(1, "" );
    LOG(1, "Point clipping?!" );
  }

  Coordinate delta = c2 - c1;
  big_double p, q, r;
  big_double u1 = 0.0, u2 = 1.0;

  for( int index = 0; index < 4; index++ )
  {
    if     (index == 0){ p = -delta.x; q = c1.x - axes.xWiMin; }
    else if(index == 1){ p =  delta.x; q = axes.xWiMax - c1.x; }
    else if(index == 2){ p = -delta.y; q = c1.y - axes.yWiMin; }
    else if(index == 3){ p =  delta.y; q = axes.yWiMax - c1.y; }

    if( p == 0 && q < 0 ) {
      return false;
    }

    r = q / p;

    if( p < 0)
    {
      if( r > u1 ) {
        u1 = r;
      }
    }
    else if( p > 0 )
    {
      if( r < u2 ) {
        u2 = r;
      }
    }
  }

  if( u1 > u2 ) { return false; }

  if( u2 < 1 ) {
    c2.x = c1.x + u2 * delta.x;
    c2.y = c1.y + u2 * delta.y;
  }

  if( u1 > 0 ) {
    c1.x = c1.x + u1 * delta.x;
    c1.y = c1.y + u1 * delta.y;
  }

  return true;
}

// Cohen–Sutherland clipping algorithm clips a line from
// P0 = (x0, y0) to P1 = (x1, y1) against a rectangle with
// diagonal from (xmin, ymin) to (xmax, ymax).
bool Line::_cohenSuthelandLineClip(const Axes& axes)
{
  this->destroyList(this->_clippingCoordinates);

  for( auto coordinate : this->_windowCoordinates )
  {
    this->_clippingCoordinates.push_back(new Coordinate(*coordinate));
  }

  auto front = this->_clippingCoordinates.begin();

  auto& c1 = **front; ++front;
  auto& c2 = **front;

  unsigned int rc1 = _getCoordianteRegionCode(axes, c1);
  unsigned int rc2 = _getCoordianteRegionCode(axes, c2);

  // LOG(8, "rc1: %s, rc2: %s", rc1,rc2);
  // LOG(8, "c1: %s, c2: %s", c1,c2);

  while( true )
  {
    // Inside - bitwise OR is 0: both points inside window; trivially accept and exit loop
    if( !(rc1 | rc2) )
    {
      // LOG(8, "return ||");
      return true;
    }
    // Out - bitwise AND is not 0: both points share an outside zone (LEFT, RIGHT, TOP,
    // or BOTTOM), so both must be outside window; exit loop (accept is false)
    else if( rc1 & rc2 )
    {
      // LOG(8, "return &&");
      return false;
    }

    // failed both tests, so calculate the line segment to clip
    // from an outside point to an intersection with clip edge
    double x, y;

    // (rc1 == 0) => Inside - At least one endpoint is outside the clip rectangle; pick it.
     unsigned int outcodeout = rc1 ? rc1 : rc2;

    // Now find the intersection point;
    // use formulas:
    //   slope = (y1 - y0) / (x1 - x0)
    //   x = x0 + (1 / slope) * (ym - y0), where ym is ymin or ymax
    //   y = y0 + slope * (xm - x0), where xm is xmin or xmax
    // No need to worry about divide-by-zero because, in each case, the
    // outcode bit being tested guarantees the denominator is non-zero
    long double m = (c2.y - c1.y) / (c2.x - c1.x);
    // LOG(8, "m: %s", m);
    // LOG(8, "outcodeout: %s", outcodeout);

    // point is above the clip window
    if( outcodeout & CohenSuthelandRegionCode::TOP )
    {
      x = c1.x + 1/m * (axes.yWiMax - c1.y);
      y = axes.yWiMax;
      // LOG(8, "CohenSuthelandRegionCode::TOP: %d", CohenSuthelandRegionCode::TOP);
    }
    // point is below the clip window
    else if( outcodeout & CohenSuthelandRegionCode::BOTTOM )
    {
      x = c1.x + 1/m * (axes.yWiMin - c1.y);
      y = axes.yWiMin;
      // LOG(8, "CohenSuthelandRegionCode::BOTTOM: %d", CohenSuthelandRegionCode::BOTTOM);
    }
    // point is to the right of clip window
    else if( outcodeout & CohenSuthelandRegionCode::RIGHT )
    {
      y = m * (axes.xWiMax-c1.x) + c1.y;
      x = axes.xWiMax;
      // LOG(8, "CohenSuthelandRegionCode::RIGHT: %d", CohenSuthelandRegionCode::RIGHT);
    }
    // point is to the left of clip window
    else if( outcodeout & CohenSuthelandRegionCode::LEFT )
    {
      y = m * (axes.xWiMin-c1.x) + c1.y;
      x = axes.xWiMin;
     // LOG(8, "CohenSuthelandRegionCode::LEFT: %d", CohenSuthelandRegionCode::LEFT);
    }

    // LOG(8, "c1: %s, c2: %s", c1,c2);

    // Now we move outside point to intersection point to clip
    // and get ready for next pass.
    if( outcodeout == rc1 )
    {

      // LOG(8, "rc1: %s, rc2: %s", rc1,rc2);
      c1.x = x;
      c1.y = y;
      rc1 = _getCoordianteRegionCode(axes, c1);
      // LOG(8, "rc1: %s, rc2: %s", rc1,rc2);

    }
    else
    {

      // LOG(8, "rc1: %s, rc2: %s", rc1,rc2);
      c2.x = x;
      c2.y = y;
      rc2 = _getCoordianteRegionCode(axes, c2);
      // LOG(8, "rc1: %s, rc2: %s", rc1,rc2);

    }
    // LOG(8, "c1: %s, c2: %s", c1,c2);
  }

  // LOG(8, "return true");
  return true;
}

unsigned int Line::_getCoordianteRegionCode(const Axes& axes, const Coordinate& c)
{
  unsigned int region_code = CohenSuthelandRegionCode::INSIDE;

  if(c.x < axes.xWiMin -0.0001) {
    region_code |= CohenSuthelandRegionCode::LEFT;
    // LOG(8, "1. Chanding region_code to: %s", region_code);
  }
  else if(c.x > axes.xWiMax+0.0001) {
    // LOG(8, "2. Chanding region_code to: %s", region_code);
    region_code |= CohenSuthelandRegionCode::RIGHT;
  }

  if(c.y < axes.yWiMin-0.0001) {
    // LOG(8, "3. Chanding region_code to: %s", region_code);
    region_code |= CohenSuthelandRegionCode::BOTTOM;
  }
  else if(c.y > axes.yWiMax+0.0001) {
    // LOG(8, "4. Chanding region_code to: %s", region_code);
    region_code |= CohenSuthelandRegionCode::TOP;
  }

  // LOG(8, "axes: %s", axes);
  // LOG(8, "axes.xWiMin: %s", axes.xWiMin);
  // LOG(8, "axes.xWiMax: %s", axes.xWiMax);
  // LOG(8, "axes.yWiMin: %s", axes.yWiMin);
  // LOG(8, "axes.yWiMax: %s", axes.yWiMax);
  // LOG(8, "region_code: %s", region_code);
  return region_code;
}
