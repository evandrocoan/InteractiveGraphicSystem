#include "transformation.h"

void Transformation::apply(Coordinate* point)
{
  LOG(4, "Applying transformation %s on %s", this->_transformation, *point);
  point->multiply(&this->_transformation);
}

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

  TransformationData transformation{name, rotation, TransformationType::ROTATION, type, coordinate};
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

  TransformationData transformation{name, scaling, TransformationType::SCALING};
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

  TransformationData transformation{name, translation, TransformationType::TRANSLATION};
  this->transformations.push_back(transformation);
}

void Transformation::set_geometric_center(Coordinate center = _default_coordinate_value_parameter)
{
  LOG(4, "Center on %s", center);
  unsigned int index = 0;

  for( auto transformation_data : this->transformations )
  {
    switch( transformation_data.type )
    {
      case TransformationType::TRANSLATION:
      {
        if( index == 0 )
        {
          this->_transformation = transformation_data.matrix;
        }
        else
        {
          this->_transformation.multiply(&transformation_data.matrix);
        }
        break;
      }

      case TransformationType::SCALING:
      {
        this->_set_scaling_data(transformation_data, index, center);
        break;
      }

      case TransformationType::ROTATION:
      {
        this->_set_scaling_data(transformation_data, index, center);
        break;
      }

      default:
      {
        LOG(1, "");
        LOG(1, "");
        LOG(1, "ERROR! Invalid TransformationData type used: %d", transformation_data.type);
      }
    }

    index++;
  }
}

void Transformation::_set_scaling_data(TransformationData &transformation_data, unsigned int &index, Coordinate &center)
{
  MatrixForm move_to_center
  {
    {1             ,  0            ,              0},
    {0             ,  1            ,              0},
    {-center.getx(), -center.gety(), -center.getz()}
  };

  if( index == 0 )
  {
    this->_transformation = move_to_center;
  }
  else
  {
    this->_transformation.multiply(&move_to_center);
  }

  // Do the scaling on the origin
  this->_transformation.multiply(&transformation_data.matrix);

  // Move back to its origin
  move_to_center[3][0] = center.getx();
  move_to_center[3][1] = center.gety();
  move_to_center[3][2] = center.getz();

  this->_transformation.multiply(&move_to_center);
}

void Transformation::_set_rotation_data(TransformationData &transformation_data, unsigned int &index, Coordinate &center)
{
  switch(transformation_data.rotation_type)
  {
    case RotationType::ON_WORLD_CENTER:
    {
      // Just rotate it, as all rotations are based on the world center
      if( index == 0 )
      {
        this->_transformation = transformation_data.matrix;
      }
      else
      {
        this->_transformation.multiply(&transformation_data.matrix);
      }
      break;
    }

    case RotationType::ON_ITS_OWN_CENTER:
    {
      this->_rotation_on_center(transformation_data, index, center);
      break;
    }

    case RotationType::ON_GIVEN_COORDINATE:
    {
      this->_rotation_on_coordinate(transformation_data, index, center);
      break;
    }

    default:
    {
      LOG(1, "");
      LOG(1, "");
      LOG(1, "ERROR! Invalid RotationType used: %d", transformation_data.rotation_type);
    }
  }
}

void Transformation::_rotation_on_center(TransformationData &transformation_data, unsigned int &index, Coordinate &center)
{
  MatrixForm move_to_center
  {
    {1             ,  0            ,              0},
    {0             ,  1            ,              0},
    {-center.getx(), -center.gety(), -center.getz()}
  };

  if( index == 0 )
  {
    this->_transformation = move_to_center;
  }
  else
  {
    this->_transformation.multiply(&move_to_center);
  }

  // Do the rotation on the origin
  this->_transformation.multiply(&transformation_data.matrix);

  // Move back to its origin
  move_to_center[3][0] = center.getx();
  move_to_center[3][1] = center.gety();
  move_to_center[3][2] = center.getz();

  this->_transformation.multiply(&move_to_center);
}


void Transformation::_rotation_on_coordinate(TransformationData &transformation_data, unsigned int &index, Coordinate &center)
{
  auto rotation_center = transformation_data.rotation_center;

  MatrixForm move_to_center
  {
    {1                      ,  0                     ,                      0},
    {0                      ,  1                     ,                      0},
    {-rotation_center.getx(), -rotation_center.gety(),-rotation_center.getz()}
  };

  if( index == 0 )
  {
    this->_transformation =move_to_center;
  }
  else
  {
    this->_transformation.multiply(&move_to_center);
  }

  // Do the rotation on the origin
  this->_transformation.multiply(&transformation_data.matrix);

  // Move back to its origin
  move_to_center[3][0] = rotation_center.getx();
  move_to_center[3][1] = rotation_center.gety();
  move_to_center[3][2] = rotation_center.getz();

  this->_transformation.multiply(&move_to_center);
}
