#include "curve.h"
#include "line.h"
#include "math.h"

std::ostream& operator<<(std::ostream &output, const CurveType object)
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

BezierCurve::BezierCurve(std::string name, std::vector<Coordinate*> _worldCoordinates,
              Coordinate _borderColor, Coordinate _fillingColor) :
      Curve(name, _worldCoordinates, _borderColor, _fillingColor)
{
  if(_worldCoordinates.size() < 4 || (_worldCoordinates.size()-4)%3 != 0)
  {
    std::ostringstream contents;
    for( auto value : _worldCoordinates ) contents << *value << ", ";

    std::string error = tfm::format(
        "A Bezier curve should have 4, 7, 10, 13... coordinates, not %s.\n\n%s",
        _worldCoordinates.size(), contents.str() );

    LOG( 1, "%s", error );
    throw std::runtime_error( error );
  }

  this->_isDrawable = true;
}

BezierCurve::~BezierCurve()
{
}

big_double Ni(int n, int index)
{
    big_double a1 = factorial(n);
    big_double a2 = factorial(index);
    big_double a3 = factorial(n - index);
    big_double ni =  a1/ (a2 * a3);
    return ni;
}

// https://en.wikipedia.org/wiki/Bernstein_polynomial
big_double bernstein(int n, int index, big_double t)
{
    big_double ti = (t == 0.0 && index == 0) ? 1.0 : pow(t, index); /* t^index */
    big_double tni = (n == index && t == 1.0) ? 1.0 : pow((1 - t), (n - index)); /* (1 - t)^index */
    big_double basis = Ni(n, index) * ti * tni; //Bernstein basis
    return basis;
}

// https://www.codeproject.com/Articles/25237/Bezier-Curves-Made-Simple
void BezierCurve::updateWindowCoordinates(const Transformation& transformation)
{
  LOG( 8, "..." );

  auto coordinates = this->worldCoordinates();
  DrawableObject::destroyList(this->lines);

  int points_count = coordinates.size();
  int points_to_draw = 100;

  Line* line;
  Coordinate* new_point;
  Coordinate* last_point = nullptr;
  std::vector<Coordinate*> generated_coordinates;

  big_double basis;
  big_double current_step = 0;
  big_double step = 1.0 / ( (big_double) ( points_to_draw - 1 ) );

  LOG(16, "points_count: %s", points_count );
  LOG(16, "step: %s", step );
  LOG(16, "points_to_draw: %s", points_to_draw );

  // Calculate points on curve
  for (int index = 0; index < points_to_draw; index++)
  {
    if ((1.0 - current_step) < 5e-6) current_step = 1.0;
    new_point = new Coordinate(0, 0);

    for (int index = 0; index < points_count; index++)
    {
      basis = bernstein(points_count - 1, index, current_step);
      new_point->x += basis * coordinates[index]->x;
      new_point->y += basis * coordinates[index]->y;

      // LOG( 8, "current_step: %s, coordinates[%s]: %s, new_point: %s", current_step, index, *coordinates[index], *new_point );
    }

    generated_coordinates.push_back( new_point );
    current_step += step;
  }

  for( auto coordinate : generated_coordinates )
  {
    if( last_point != nullptr )
    {
      // LOG( 8, "last_point: %s", *last_point );
      // LOG( 8, "point:      %s", *coordinate );

      line = new Line( "Bezier", last_point, coordinate, _default_coordinate_value_parameter, LineClippingType::LIANG_BARSKY, false );
      line->updateWindowCoordinates( transformation );

      // LOG( 8, "line:       %s", *line );
      this->lines.push_back( line );
    }

    last_point = coordinate;
  }
}

void BezierCurve::updateClippingCoordinates(const Axes& axes)
{
  LOG( 8, "..." );
  for( auto line : this->lines )
  {
    line->updateClippingCoordinates(axes);
  }
}

