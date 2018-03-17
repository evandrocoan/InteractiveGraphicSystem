#include "transformation.h"

/**
 * Create and configure correctly a rotation.
 *
 * @param degrees [description]
 * @param type    an enum RotationType valid value
 * @param point   this is a optional value, only required when using RotationType::ON_GIVEN_COORDINATE
 */
void Transformation::add_rotation(std::string name, double degrees, RotationType type, Coordinate coordinate=_default_coordinate_value_parameter)
{
  MatrixForm rotation =
  {
    {(GTKMM_APP_MATRIX_FORM_H_DATATYPE)std::cos(degrees), -(GTKMM_APP_MATRIX_FORM_H_DATATYPE)std::sin(degrees), 0},
    {(GTKMM_APP_MATRIX_FORM_H_DATATYPE)std::sin(degrees),  (GTKMM_APP_MATRIX_FORM_H_DATATYPE)std::cos(degrees), 0},
    {0                                                  ,  0                                                  , 1}
  };

  switch(type)
  {
    case RotationType::ON_ITS_OWN_CENTER:
    {
      if( !coordinate.is_initialized )
      {
        LOG(1, "");
        LOG(1, "");
        LOG(1, "ERROR! Invalid Coordinate must to be NULL, instead of: %s", coordinate);
      }
      break;
    }

    case RotationType::ON_WORLD_CENTER:
    {
      coordinate = Coordinate{0,0,0};
      break;
    }

    case RotationType::ON_GIVEN_COORDINATE:
    {
      if( coordinate.is_initialized )
      {
        LOG(1, "");
        LOG(1, "");
        LOG(1, "ERROR! Invalid Coordinate used: NULL(%s)", coordinate);
      }
    }

    default:
    {
      LOG(1, "");
      LOG(1, "");
      LOG(1, "ERROR! Invalid RotationType used: %d", type);
    }
  }

  TransformationData transformation{name, rotation, coordinate};
  this->transformations.push_back(transformation);
}

void Transformation::add_scaling(std::string name, Coordinate move)
{
  MatrixForm scaling =
  {
    {move.getx(), 0          , 0          },
    {0          , move.gety(), 0          },
    {0          , 0          , move.getz()}
  };

  TransformationData transformation{name, scaling};
  this->transformations.push_back(transformation);
}

void Transformation::add_translation(std::string name, Coordinate movement)
{
  MatrixForm translation =
  {
    {1              , 0              ,               0},
    {0              , 1              ,               0},
    {movement.getx(), movement.gety(), movement.getz()}
  };

  TransformationData transformation{name, translation};
  this->transformations.push_back(transformation);
}

void Transformation::set_geometric_center(Coordinate center)
{
  LOG(4, "Center on %s", center);
}

void Transformation::apply(Coordinate point)
{
  LOG(4, "Applying transformation %s on %s", this->_transformation, point);
  point.multiply(this->_transformation);
}

