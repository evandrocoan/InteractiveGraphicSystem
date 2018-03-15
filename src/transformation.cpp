#include "transformation.h"

Transformation::Transformation(std::string name) :
      name(name),
      transformations()
{
}

/**
 * Create and configure correctly a rotation.
 *
 * @param degrees [description]
 * @param type    an enum RotationType valid value
 * @param point   this is a optional value, only required when using RotationType::ON_GIVEN_COORDINATE
 */
void Transformation::add_rotation(double degrees, RotationType type, Coordinate point=NULL)
{

  switch(type)
  {
    case RotationType::ON_ITS_OWN_CENTER:
    {
      long int rotation[3][3] =
      {
        {cos(degree), -sin(degree), 0},
        {sin(degree),  cos(degree), 0},
        {0          ,  0          , 1}
      };

      this->rotations.push_back(rotation);
      break;
    }

    case RotationType::ON_WORLD_CENTER:
    {
      break;
    }

    case RotationType::ON_GIVEN_COORDINATE:
    {
      break;
    }

    default:
    {
      LOG(1, "");
      LOG(1, "");
      LOG(1, "ERROR! Invalid RotationType used: %d", type);
    }
  }
}

void Transformation::add_scaling(double scale)
{
  long int scaling[3][3] =
  {
    {scale, 0      , 0},
    {0      , scale, 0},
    {0      ,  0   , 1}
  };

  this->scalings.push_back(scaling);
}

void Transformation::add_translation(Coordinate move)
{
  long int transformation[3][3] =
  {
    {1,         , 0          ,           0},
    {0          , 1          ,           0},
    {move.getx(), move.gety(), move.getz()}
  };

  this->transformations.push_back(transformation);
}

void Transformation::set_geometric_center(Coordinate center)
{

}

long int[3][3] Transformation::multiply_3x3_matrix(long int coordinates[3][3], long int transformation[3][3])
{
  int line;
  int column;
  int step;
  long int result[3][3] = { 0 };

  for(line = 0; line < 3; line++)
  {
    for(column = 0; column < 3; column++)
    {
      for(step = 0; step < 3; step++)
      {
        result[line][column] += coordinates[line][step] * transformation[step][column];
      }
    }
  }

  return result;
}

long int[3] Transformation::multiply_1x3_matrix(long int coordinates[3], long int transformation[3][3])
{
  int column;
  int step;
  long int result[3] = { 0 };

  for(column = 0; column < 3; column++)
  {
    for(step = 0; step < 3; step++)
    {
      result[column] += coordinates[step] * transformation[step][column];
    }
  }

  return result;
}
