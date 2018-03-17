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
void Transformation::add_rotation(double degrees, RotationType type, Coordinate coordinate=_default_coordinate_value_parameter)
{
  MatrixForm rotation =
  {
    {(GTKMM_APP_MATRIX_FORM_H_DATATYPE)std::cos(degrees), -(GTKMM_APP_MATRIX_FORM_H_DATATYPE)std::sin(degrees), 0},
    {(GTKMM_APP_MATRIX_FORM_H_DATATYPE)std::sin(degrees),  (GTKMM_APP_MATRIX_FORM_H_DATATYPE)std::cos(degrees), 0},
    {0                ,  0                , 1}
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

  TransformationData transformation{rotation, coordinate};
  this->transformations.push_back(transformation);
}

void Transformation::add_scaling(Coordinate move)
{
  MatrixForm scaling =
  {
    {move.getx(), 0          , 0          },
    {0          , move.gety(), 0          },
    {0          , 0          , move.getz()}
  };

  TransformationData transformation{scaling};
  this->transformations.push_back(transformation);
}

void Transformation::add_translation(Coordinate move)
{
  MatrixForm translation =
  {
    {1          , 0          ,           0},
    {0          , 1          ,           0},
    {move.getx(), move.gety(), move.getz()}
  };

  TransformationData transformation{translation};
  this->transformations.push_back(transformation);
}

void Transformation::set_geometric_center(Coordinate center)
{

}

void Transformation::apply(Coordinate center)
{
  center.multiply(this->_transformation);
}

