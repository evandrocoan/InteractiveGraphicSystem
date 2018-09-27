#include "curve.h"
#include "line.h"

Curve::Curve(std::string name, std::list<Coordinate*> _worldCoordinates,
              Coordinate _borderColor, Coordinate _fillingColor, CurveType type) :
      DrawableObject(name, _worldCoordinates, _borderColor, _fillingColor),
      visible_on_gui(false),
      curve_type(type)
{
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
  auto front = this->_clippingCoordinates.begin();

  auto& c1 = **front; ++front;
  auto& c2 = **front;

  if( c1 == c2 )
  {
    // Point clipping?!
  }

  LOG(16, "c1: %s, c2: %s", c1, c2);
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
