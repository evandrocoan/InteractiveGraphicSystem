#include "curve.h"
#include "line.h"
#include "math.h"

Curve::Curve(std::string name, std::vector<Coordinate*> _worldCoordinates,
              Coordinate _borderColor, Coordinate _fillingColor, CurveType type) :
      DrawableObject(name, _worldCoordinates, _borderColor, _fillingColor, type),
      visible_on_gui(false)
{
  if( curve_type == BEZIER )
  {
    if(_worldCoordinates.size() < 4 || (_worldCoordinates.size()-4)%3 != 0)
    {
      std::string error = tfm::format( "A Bezier curve should have 4, 7, 10, 13... coordinates, not %s.", _worldCoordinates.size() );
      throw std::runtime_error( error );
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

void Curve::updateWindowCoordinates(const Transformation& transformation)
{
  LOG(4, "Curve window update... %s %s", this->curve_type, transformation);
  switch( this->curve_type )
  {
    case CurveType::BEZIER:  this->_isDrawable = this->_bezier(transformation); break;
    case CurveType::BSPLINE: this->_isDrawable = this->_bspline(transformation); break;
    default:
    {
      std::string error = tfm::format( "ERROR! Invalid CurveType algorithm passed by: %s", this->curve_type );
      throw std::runtime_error( error );
    }
  }
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
      std::string error = tfm::format( "ERROR! Invalid CurveType algorithm passed by: %s", this->curve_type );
      throw std::runtime_error( error );
    }
  }
}

double factorial(int n)
{
    double x = 1.0;
    for (int index = 1; index <= n; index++)
      x *= (double)index;
    return x;
}

double Ni(int n, int index)
{
    double a1 = factorial(n);
    double a2 = factorial(index);
    double a3 = factorial(n - index);
    double ni =  a1/ (a2 * a3);
    return ni;
}

// https://en.wikipedia.org/wiki/Bernstein_polynomial
double bernstein(int n, int index, double t)
{
    double ti = (t == 0.0 && index == 0) ? 1.0 : pow(t, index); /* t^index */
    double tni = (n == index && t == 1.0) ? 1.0 : pow((1 - t), (n - index)); /* (1 - t)^index */
    double basis = Ni(n, index) * ti * tni; //Bernstein basis
    return basis;
}

// https://www.codeproject.com/Articles/25237/Bezier-Curves-Made-Simple
bool Curve::_bezier(const Transformation& transformation)
{
  LOG(8, "Entering... %s", transformation);

  auto coordinates = this->worldCoordinates();
  DrawableObject::destroyList(this->lines);

  int npts = coordinates.size() / 2;
  int points_to_draw = 10;

  Line* line;
  Coordinate* new_point;
  Coordinate* last_point = nullptr;
  std::vector<Coordinate*> generated_coordinates;

  double x, y;
  double basis;
  double current_step = 0;
  double step = 1.0 / ( (double) ( points_to_draw - 1 ) );

  LOG(1, "npts: %s", npts );
  LOG(1, "step: %s", step );
  LOG(1, "points_to_draw: %s", points_to_draw );

  // Calculate points on curve
  for (int index = 0; index != points_to_draw; index++)
  {
    if ((1.0 - current_step) < 5e-6) current_step = 1.0;

    for (int index = 0; index < npts; index++)
    {
      basis = bernstein(npts - 1, index, current_step);
      x = basis * coordinates[index]->x;
      y = basis * coordinates[index]->y;

      new_point = new Coordinate(x, y);
      generated_coordinates.push_back( new_point );

      LOG( 1, "current_step: %s, coordinates[%s]: %s, new_point: %s", current_step, index, *coordinates[index], *new_point );
    }

    current_step += step;
  }

  for( auto coordinate : generated_coordinates )
  {
    if( last_point != nullptr )
    {
      LOG( 1, "last_point: %s", *last_point );
      LOG( 1, "point:      %s", *coordinate );

      line = new Line( "Bezier", last_point, coordinate, _default_coordinate_value_parameter, LineClippingType::LIANG_BARSKY, false );
      line->updateWindowCoordinates( transformation );

      LOG( 1, "line:       %s", *line );
      this->lines.push_back( line );
    }

    last_point = coordinate;
  }

  return true;
}

bool Curve::_bezier(const Axes& axes)
{
  for( auto line : this->lines )
  {
    line->updateClippingCoordinates(axes);
  }

  LOG(16, "return true");
  return true;
}

bool Curve::_bspline(const Transformation& transformation)
{
  LOG(16, "return true");
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
