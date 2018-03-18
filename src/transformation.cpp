#include "transformation.h"

void Transformation::apply(Coordinate* point)
{
  LOG(4, "Applying transformation %s on %s", this->_transformation, *point);
  point->multiply(&this->_transformation);
}

void Transformation::remove_transformation(std::string name)
{
  for( auto iterator = this->transformations.begin(); iterator != this->transformations.end(); iterator++ )
  {
    if( (*iterator).name == name )
    {
      this->transformations.erase(iterator);
      return;
    }
  }
}

std::vector<TransformationData>& Transformation::getTransformations()
{
  return this->transformations;
}

unsigned int Transformation::size()
{
  return this->transformations.size();
}

/**
 * Prints a more beauty version of the transformations when called on `std::cout`
 */
std::ostream& operator<<( std::ostream &output, const Transformation &object )
{
  output << object._transformation << "(";

  unsigned int index = 0;
  unsigned int size = object.transformations.size() - 1;

  for( auto transformation_data : object.transformations )
  {
    output << transformation_data;

    if( index != size )
    {
      output << ", ";
    }

    index++;
  }

  output << ")";
  return output;
}

void Transformation::add_rotation(std::string name, Array<3, long double> degrees, Coordinate coordinate, RotationType type)
{
  MatrixForm rotation =
  {
    {(GTKMM_APP_MATRIX_FORM_H_DATATYPE)std::cos(degrees[0]), -(GTKMM_APP_MATRIX_FORM_H_DATATYPE)std::sin(degrees[0]), 0},
    {(GTKMM_APP_MATRIX_FORM_H_DATATYPE)std::sin(degrees[0]),  (GTKMM_APP_MATRIX_FORM_H_DATATYPE)std::cos(degrees[0]), 0},
    {0                                                     ,  0                                                     , 1}
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
