#include "transformation.h"

Transformation::Transformation() :
    _transformation{}
{
  this->clear();
}

Transformation::~Transformation()
{
}

void Transformation::apply(Coordinate &point)
{
  LOG(4, "Applying transformation %s on %s", this->_transformation, point);

  if( this->isInitialized )
  {
    point.multiply(this->_transformation);
    LOG(4, "point.multiply: %s", point);
  }
}

void Transformation::clear()
{
  this->isInitialized = false;
  this->transformations.clear();
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
  unsigned int index = 0;
  unsigned int size = object.transformations.size() - 1;

  output << "(" << object._transformation << ") ";

  for( auto transformation_data : object.transformations )
  {
    output << transformation_data;

    if( index != size )
    {
      output << " - ";
    }

    index++;
  }

  return output;
}

void Transformation::add_rotation(std::string name, Coordinate degrees, Coordinate coordinate, TransformationPoint type)
{
  MatrixForm rotation =
  {
    {std::cos(convert_degrees_to_radians(degrees[0])), -std::sin(convert_degrees_to_radians(degrees[0])), 0},
    {std::sin(convert_degrees_to_radians(degrees[0])),  std::cos(convert_degrees_to_radians(degrees[0])), 0},
    {0                                               ,  0                                               , 1}
  };

  TransformationData transformation{name, rotation, TransformationType::ROTATION, type, coordinate};
  this->transformations.push_back(transformation);
}

void Transformation::add_scaling(std::string name, Coordinate move)
{
  MatrixForm scaling =
  {
    {move.x     , 0     , 0},
    {0          , move.y, 0},
    {0          , 0     , 1}
  };

  TransformationData transformation{name, scaling, TransformationType::SCALING};
  this->transformations.push_back(transformation);
}

void Transformation::add_translation(std::string name, Coordinate movement)
{
  MatrixForm translation =
  {
    {1         , 0         , 0},
    {0         , 1         , 0},
    {movement.x, movement.y, 1}
  };

  TransformationData transformation{name, translation, TransformationType::TRANSLATION};
  this->transformations.push_back(transformation);
}

void Transformation::set_geometric_center(Coordinate &center)
{
  LOG(4, "Center on %s - %s", center, *this);
  unsigned int index = 0;

  for( auto transformation_data : this->transformations )
  {
    switch( transformation_data.type )
    {
      case TransformationType::TRANSLATION:
      {
        this->_set_translation_data(transformation_data, index, center);
        break;
      }

      case TransformationType::SCALING:
      {
        this->_set_scaling_data(transformation_data, index, center);
        break;
      }

      case TransformationType::ROTATION:
      {
        this->_set_rotation_data(transformation_data, index, center);
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
    this->isInitialized = true;
  }
}

void Transformation::_set_translation_data(TransformationData &transformation_data, unsigned int &index, Coordinate &center)
{
  if( index == 0 )
  {
    this->_transformation = transformation_data.matrix;
  }
  else
  {
    this->_transformation.multiply(transformation_data.matrix);
  }
  LOG(4, "_transformation.multiply: %s", this->_transformation);
}

void Transformation::_set_scaling_data(TransformationData &transformation_data, unsigned int &index, Coordinate &center)
{
  LOG(2, "Entering...");

  switch(transformation_data.point)
  {
    case TransformationPoint::ON_WORLD_CENTER:
    {
      this->_scaling_on_world_center(transformation_data, index, center);
      break;
    }

    case TransformationPoint::ON_ITS_OWN_CENTER:
    {
      this->_scaling_on_its_own_center(transformation_data, index, center);
      break;
    }

    case TransformationPoint::ON_GIVEN_COORDINATE:
    {
      this->_scaling_on_coordinate(transformation_data, index, center);
      break;
    }

    default:
    {
      LOG(1, "");
      LOG(1, "");
      LOG(1, "ERROR! Invalid TransformationPoint used: %d", transformation_data.point);
    }
  }
}

void Transformation::_scaling_on_world_center(TransformationData &transformation_data, unsigned int &index, Coordinate &center)
{
  LOG(4, "Just rotate it, as all scalings are based on the world center");
  if( index == 0 )
  {
    this->_transformation = transformation_data.matrix;
  }
  else
  {
    this->_transformation.multiply(transformation_data.matrix);
  }
}

void Transformation::_scaling_on_its_own_center(TransformationData &transformation_data, unsigned int &index, Coordinate &center)
{
  LOG(2, "Entering...");

  MatrixForm move_to_center
  {
    {1        ,  0       , 0},
    {0        ,  1       , 0},
    {-center.x, -center.y, 1}
  };

  if( index == 0 )
  {
    this->_transformation = move_to_center;
  }
  else
  {
    this->_transformation.multiply(move_to_center);
  }

  LOG(4, "Do the scaling on the origin");
  this->_transformation.multiply(transformation_data.matrix);

  LOG(4, "Move back to its origin");
  move_to_center[2][0] = center.x;
  move_to_center[2][1] = center.y;
  move_to_center[2][2] = 1;

  this->_transformation.multiply(move_to_center);
}

void Transformation::_scaling_on_coordinate(TransformationData &transformation_data, unsigned int &index, Coordinate &center)
{
  LOG(2, "Entering...");

  MatrixForm move_to_center
  {
    {1                            ,  0                           , 0},
    {0                            ,  1                           , 0},
    {-transformation_data.center.x, -transformation_data.center.y, 1}
  };

  if( index == 0 )
  {
    this->_transformation = move_to_center;
  }
  else
  {
    this->_transformation.multiply(move_to_center);
  }

  LOG(4, "Do the scaling on the origin");
  this->_transformation.multiply(transformation_data.matrix);

  LOG(4, "Move back to its origin");
  move_to_center[2][0] = center.x;
  move_to_center[2][1] = center.y;
  move_to_center[2][2] = 1;

  this->_transformation.multiply(move_to_center);
}

void Transformation::_set_rotation_data(TransformationData &transformation_data, unsigned int &index, Coordinate &center)
{
  LOG(2, "Entering...");

  switch(transformation_data.point)
  {
    case TransformationPoint::ON_WORLD_CENTER:
    {
      this->_rotation_on_world_center(transformation_data, index, center);
      break;
    }

    case TransformationPoint::ON_ITS_OWN_CENTER:
    {
      this->_rotation_on_its_own_center(transformation_data, index, center);
      break;
    }

    case TransformationPoint::ON_GIVEN_COORDINATE:
    {
      this->_rotation_on_coordinate(transformation_data, index, center);
      break;
    }

    default:
    {
      LOG(1, "");
      LOG(1, "");
      LOG(1, "ERROR! Invalid TransformationPoint used: %d", transformation_data.point);
    }
  }
}

void Transformation::_rotation_on_world_center(TransformationData &transformation_data, unsigned int &index, Coordinate &center)
{
  LOG(4, "Just rotate it, as all rotations are based on the world center");
  if( index == 0 )
  {
    this->_transformation = transformation_data.matrix;
  }
  else
  {
    this->_transformation.multiply(transformation_data.matrix);
  }
}

void Transformation::_rotation_on_its_own_center(TransformationData &transformation_data, unsigned int &index, Coordinate &center)
{
  LOG(2, "Entering...");

  MatrixForm move_to_center
  {
    {1        ,  0       , 0},
    {0        ,  1       , 0},
    {-center.x, -center.y, 1}
  };

  if( index == 0 )
  {
    this->_transformation = move_to_center;
  }
  else
  {
    this->_transformation.multiply(move_to_center);
  }

  LOG(4, "Do the rotation on the origin");
  this->_transformation.multiply(transformation_data.matrix);

  LOG(4, "Move back to its origin");
  move_to_center[2][0] = center.x;
  move_to_center[2][1] = center.y;
  move_to_center[2][2] = 1;

  this->_transformation.multiply(move_to_center);
}


void Transformation::_rotation_on_coordinate(TransformationData &transformation_data, unsigned int &index, Coordinate &center)
{
  LOG(2, "Entering...");

  MatrixForm move_to_center
  {
    {1                            ,  0                           , 0},
    {0                            ,  1                           , 0},
    {-transformation_data.center.x, -transformation_data.center.y, 1}
  };

  if( index == 0 )
  {
    this->_transformation = move_to_center;
  }
  else
  {
    this->_transformation.multiply(move_to_center);
  }

  LOG(4, "Do the rotation on the origin");
  this->_transformation.multiply(transformation_data.matrix);

  LOG(4, "Move back to its origin");
  move_to_center[2][0] = center.x;
  move_to_center[2][1] = center.y;
  move_to_center[2][2] = 1;

  this->_transformation.multiply(move_to_center);
}
