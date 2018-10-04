#include "curve.h"
#include "line.h"
#include "math.h"

Curve::Curve(std::string name, std::vector<Coordinate*> _worldCoordinates,
              Coordinate _borderColor, Coordinate _fillingColor, CurveType type) :
      DrawableObject(name, _worldCoordinates, _borderColor, _fillingColor),
      visible_on_gui(false),
      curve_type(type)
{
  if( curve_type == BEZIER )
  {
    if(_worldCoordinates.size() < 4 || (_worldCoordinates.size()-4)%3 != 0)
    {
      throw std::runtime_error("A Bezier curve should have 4, 7, 10, 13... coordinates.");
    }
  }
}

Curve::~Curve()
{
}

inline std::ostream& operator<<(std::ostream &output, const CurveType object)
{
  switch( object )
  {
    case CurveType::BEZIER:  output << "BEZIER"; break;
    case CurveType::BSPLINE: output << "BSPLINE"; break;
    default:
      output << (int) object; break;
  }
  return output;
}

void Curve::updateClippingCoordinates(const Axes& axes)
{
  LOG(4, "Curve clipping update... %s %s", this->curve_type, axes);
  switch( this->curve_type )
  {
    case CurveType::BEZIER:  this->_isDrawable = this->_bezier(axes); break;
    case CurveType::BSPLINE: this->_isDrawable = this->_bspline(axes); break;
    default:
    {
      LOG(1, "");
      LOG(1, "");
      LOG(1, "ERROR! Invalid CurveType algorithm passed by: %s", this->curve_type);
    }
  }
}

bool Curve::_bezier(const Axes& axes)
{
  this->destroyList(this->_clippingCoordinates);

  for( auto coordinate : this->_windowCoordinates )
  {
    this->_clippingCoordinates.push_back(new Coordinate(*coordinate));
  }

  int npts = (_clippingCoordinates.size()) / 2;
  int control_points_count = _clippingCoordinates.size();
  int line_count, column_count;
  double temporary;
  const double step = 0.01;

  // Calculate points on curve
  line_count = 0;
  temporary = 0;
  for (int index = 0; index != control_points_count; index++)
  {
    if ((1.0 - temporary) < 5e-6)
        temporary = 1.0;

    column_count = 0;
    double x,y;

    double basis = bernstein(npts - 1, 0, temporary);
    x = basis * _clippingCoordinates[column_count];
    y = basis * _clippingCoordinates[column_count + 1];
    last_point = new Coordinate(x, y);

    for (int i = 1; i != npts; i++)
    {
      basis = bernstein(npts - 1, i, temporary);
      x = basis * _clippingCoordinates[column_count];
      y = basis * _clippingCoordinates[column_count + 1];
      new_point = new Coordinate(x,y)
      this->lines.push_back(new Line("name", last_point, new_point));
      last_point = new_point;
      column_count = column_count +2;
    }
    line_count += 2;
    temporary += step;
  }

  return true;
}

bool Curve::_bspline(const Axes& axes)
{
  this->destroyList(this->_clippingCoordinates);

  for( auto coordinate : this->_windowCoordinates )
  {
    this->_clippingCoordinates.push_back(new Coordinate(*coordinate));
  }

  auto front = this->_clippingCoordinates.begin();

  auto& c1 = **front; ++front;
  auto& c2 = **front;

  LOG(16, "c1: %s, c2: %s", c1, c2);
  LOG(16, "return true");
  return true;
}
