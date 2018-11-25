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

Curve::~Curve()
{
  for( auto line : this->lines )
  {
    delete line;
  }
}

void Curve::create_lines(const Transformation& transformation, std::vector<Coordinate*> generated_coordinates) {
  Line* line;
  Coordinate* last_point = nullptr;

  for( auto coordinate : generated_coordinates )
  {
    if( last_point != nullptr ) {
      // LOG( 8, "last_point: %s", *last_point );
      // LOG( 8, "point:      %s", *coordinate );

      line = new Line( "Bezier", last_point, coordinate, _origin_coordinate_value, LineClippingType::LIANG_BARSKY, false );
      line->updateWindowCoordinates( transformation );

      // LOG( 8, "line:       %s", *line );
      this->lines.push_back( line );
    }

    last_point = coordinate;
  }
}

const std::vector<Line*> Curve::getLines() const {
  return this->lines;
}

void Curve::updateClippingCoordinates(const Axes& axes) {
  LOG( 8, "..." );

  for( auto line : this->lines )
  {
    line->updateClippingCoordinates(axes);
  }
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

  Coordinate* new_point;
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
    new_point = new Coordinate(0, 0, 0);

    for (int index = 0; index < points_count; index++)
    {
      basis = bernstein(points_count - 1, index, current_step);
      new_point->x += basis * coordinates[index]->x;
      new_point->y += basis * coordinates[index]->y;
      new_point->z += basis * coordinates[index]->z;

      // LOG( 8, "current_step: %s, coordinates[%s]: %s, new_point: %s", current_step, index, *coordinates[index], *new_point );
    }

    generated_coordinates.push_back( new_point );
    current_step += step;
  }

  this->create_lines(transformation, generated_coordinates);
}


BsplineCurve::BsplineCurve(std::string name, std::vector<Coordinate*> _worldCoordinates,
              Coordinate _borderColor, Coordinate _fillingColor) :
      Curve(name, _worldCoordinates, _borderColor, _fillingColor)
{
  if(_worldCoordinates.size() < 3)
  {
    std::ostringstream contents;
    for( auto value : _worldCoordinates ) contents << *value << ", ";

    std::string error = tfm::format(
        "A Bspline curve should have at least 3 coordinates, not %s.\n\n%s",
        _worldCoordinates.size(), contents.str() );

    LOG( 1, "%s", error );
    throw std::runtime_error( error );
  }

  this->_isDrawable = true;
}

// https://www.codeproject.com/Articles/25237/Bezier-Curves-Made-Simple
void BsplineCurve::updateWindowCoordinates(const Transformation& transformation) {
  LOG( 8, "..." );

  big_double blending_function_step = 0.1;
  std::vector<Coordinate*> generated_coordinates;

  big_double t = blending_function_step;
  big_double t2 = t * blending_function_step;
  big_double t3 = t2 * blending_function_step;

  auto coordinates = this->worldCoordinates();
  DrawableObject::destroyList(this->lines);

  int points_count = coordinates.size();
  int curves_count = points_count - 3;

  for( int i = 0; i < curves_count; i++ )
  {
    Coordinate* c1 = coordinates[i];
    Coordinate* c2 = coordinates[i+1];
    Coordinate* c3 = coordinates[i+2];
    Coordinate* c4 = coordinates[i+3];

    big_double ax = -1.0/6.0 * c1->x  + 0.5 * c2->x     - 0.5     * c3->x + 1.0/6.0 * c4->x;
    big_double bx =  0.5     * c1->x  -       c2->x     + 0.5     * c3->x;
    big_double cx = -0.5     * c1->x                    + 0.5     * c3->x;
    big_double dx =  1.0/6.0 * c1->x  + 2.0/3.0 * c2->x + 1.0/6.0 * c3->x;

    big_double delta_x1 = ax * t3 + bx * t2 + cx * t;
    big_double delta_x3 = ax * 6 * t3;
    big_double delta_x2 = delta_x3 + bx * 2 * t2;

    big_double ay = -1.0/6.0 * c1->y  + 0.5 * c2->y     - 0.5     * c3->y + 1.0/6.0 * c4->y;
    big_double by =  0.5     * c1->y  -       c2->y     + 0.5     * c3->y;
    big_double cy = -0.5     * c1->y                    + 0.5     * c3->y;
    big_double dy =  1.0/6.0 * c1->y  + 2.0/3.0 * c2->y + 1.0/6.0 * c3->y;

    big_double delta_y1 = ay * t3 + by * t2 + cy * t;
    big_double delta_y3 = ay * ( 6 * t3 );
    big_double delta_y2 = delta_y3 + by * ( 2 * t2 );

    big_double az = -1.0/6.0 * c1->z  + 0.5 * c2->z     - 0.5     * c3->z + 1.0/6.0 * c4->z;
    big_double bz =  0.5     * c1->z  -       c2->z     + 0.5     * c3->z;
    big_double cz = -0.5     * c1->z                    + 0.5     * c3->z;
    big_double dz =  1.0/6.0 * c1->z  + 2.0/3.0 * c2->z + 1.0/6.0 * c3->z;

    big_double delta_z1 = az * t3 + bz * t2 + cz * t;
    big_double delta_z3 = az * ( 6 * t3 );
    big_double delta_z2 = delta_z3 + bz * ( 2 * t2 );

    big_double vx = dx, vy = dy, vz = dz;
    generated_coordinates.push_back( new Coordinate( vx, vy, dz ) );

    for( float t = 0.0; t < 1.0; t += blending_function_step )
    {
      big_double x = vx;
      big_double y = vy;
      big_double z = vz;

      x += delta_x1;
      delta_x1 += delta_x2;
      delta_x2 += delta_x3;

      y += delta_y1;
      delta_y1 += delta_y2;
      delta_y2 += delta_y3;

      z += delta_z1;
      delta_z1 += delta_z2;
      delta_z2 += delta_z3;

      generated_coordinates.push_back( new Coordinate( x, y, z ) );

      vx = x;
      vy = y;
      vz = z;
    }
  }

  this->create_lines(transformation, generated_coordinates);
}

