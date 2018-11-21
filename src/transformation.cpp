#include "transformation.h"

Transformation::Transformation() :
    _transformation{}
{
  this->clear();
}

Transformation::~Transformation()
{
}

void Transformation::apply(Coordinate &point) const
{
  // LOG(8, "Apply transformation: %s", point);

  if( this->isInitialized )
  {
    // point.z = 1.0; // No 3D support yet
    point.multiply(this->_transformation);
    // LOG(8, "Transformation result %s", point);
  }
}

void Transformation::clear()
{
  this->isInitialized = false;
  this->transformations.clear();
}

const MatrixForm Transformation::_get_translation_matrix(const Coordinate& moves) const
{
  return
  {
    {1      , 0      ,       0, 0},
    {0      , 1      ,       0, 0},
    {0      , 0      ,       1, 0},
    {moves.x, moves.y, moves.z, 1}
  };
}

const MatrixForm Transformation::_get_scaling_matrix(const Coordinate& factors) const
{
  return
  {
    {factors.x, 0        ,         0, 0},
    {0        , factors.y,         0, 0},
    {0        , 0        , factors.z, 0},
    {0        , 0        , 0        , 1}
  };
}

const MatrixForm Transformation::_get_x_rotation_matrix(const big_double& degrees) const
{
  auto radians = convert_degrees_to_radians(degrees);
  auto sine    = std::sin(radians);
  auto cosine  = std::cos(radians);
  return
    {
      {1, 0     , 0     ,   0},
      {0, cosine, sine  ,   0},
      {0, -sine , cosine,   0},
      {0, 0     , 0     ,   1}
    };
}

const MatrixForm Transformation::_get_y_rotation_matrix(const big_double& degrees) const
{
  auto radians = convert_degrees_to_radians(degrees);
  auto sine    = std::sin(radians);
  auto cosine  = std::cos(radians);
  return
    {
      {cosine, 0,  -sine, 0},
      {0     , 1,      0, 0},
      {sine  , 0, cosine, 0},
      {0     , 0,      0, 1}
    };
}

const MatrixForm Transformation::_get_z_rotation_matrix(const big_double& degrees) const
{
  auto radians = convert_degrees_to_radians(degrees);
  auto sine    = std::sin(radians);
  auto cosine  = std::cos(radians);
  return
    {
      {cosine,   sine, 0, 0},
      {-sine , cosine, 0, 0},
      {0     ,      0, 1, 0},
      {0     ,      0, 0, 1}
    };
}

void Transformation::add_rotation(const std::string name, const Coordinate degrees, const TransformationPoint point, const Coordinate center)
{
  transformations.push_back( TransformationData{name, _get_x_rotation_matrix(degrees.x), TransformationType::ROTATION, point, center} );
  transformations.push_back( TransformationData{name, _get_y_rotation_matrix(degrees.y), TransformationType::ROTATION, point, center} );
  transformations.push_back( TransformationData{name, _get_z_rotation_matrix(degrees.z), TransformationType::ROTATION, point, center} );
}

void Transformation::add_axis_rotation(const std::string name, const big_double degrees)
{
  TransformationData transformation{name, _get_z_rotation_matrix(degrees),
      TransformationType::ROTATION, TransformationPoint::ON_ITS_OWN_AXIS, _default_coordinate_value_parameter};

  this->transformations.push_back(transformation);
}

void Transformation::add_scaling(const std::string name, const Coordinate factors)
{
  TransformationData transformation{name, this->_get_scaling_matrix(factors), TransformationType::SCALING};
  this->transformations.push_back(transformation);
}

void Transformation::add_translation(const std::string name, const Coordinate moves)
{
  TransformationData transformation{name, _get_translation_matrix(moves), TransformationType::TRANSLATION};
  this->transformations.push_back(transformation);
}

void Transformation::set_geometric_center(const Coordinate &center)
{
  LOG( 16, "..." );
  // LOG( 16, "Center on %s - %s", center, *this );
  unsigned int index = 0;

  for( auto data : this->transformations )
  {
    switch( data.type )
    {
      case TransformationType::TRANSLATION:
      {
        this->_set_translation_data(data, index, center);
        break;
      }

      case TransformationType::SCALING:
      {
        this->_set_scaling_data(data, index, center);
        break;
      }

      case TransformationType::ROTATION:
      {
        this->_set_rotation_data(data, index, center);
        break;
      }

      default:
      {
        LOG(1, "");
        LOG(1, "");
        LOG(1, "ERROR! Invalid TransformationData type used: %d", data.type);
      }
    }

    index++;
    this->isInitialized = true;
  }
}

void Transformation::_set_translation_data(const TransformationData &data, const unsigned int &index, const Coordinate &center)
{
  if( index == 0 )
  {
    this->_transformation = data.matrix;
  }
  else
  {
    this->_transformation.multiply(data.matrix);
  }
  // LOG(16, "_transformation.multiply: %s", this->_transformation);
}

void Transformation::_set_scaling_data(const TransformationData &data, const unsigned int &index, const Coordinate &center)
{
  LOG(16, "...");

  switch(data.point)
  {
    case TransformationPoint::ON_WORLD_CENTER:
    {
      this->_scaling_on_world_center(data, index, center);
      break;
    }

    case TransformationPoint::ON_ITS_OWN_CENTER:
    {
      this->_scaling_on_its_own_center(data, index, center);
      break;
    }

    case TransformationPoint::ON_GIVEN_COORDINATE:
    {
      this->_scaling_on_coordinate(data, index, center);
      break;
    }

    default:
    {
      LOG(1, "");
      LOG(1, "");
      LOG(1, "ERROR! Invalid TransformationPoint used: %d", data.point);
    }
  }
}

void Transformation::_scaling_on_world_center(const TransformationData &data, const unsigned int &index, const Coordinate &center)
{
  // LOG(16, "Just rotate it, as all scalings are based on the world center");
  if( index == 0 )
  {
    this->_transformation = data.matrix;
  }
  else
  {
    this->_transformation.multiply(data.matrix);
  }
}

void Transformation::_scaling_on_its_own_center(const TransformationData &data, const unsigned int &index, const Coordinate &center)
{
  LOG(2, "...");
  MatrixForm move_to_center = this->_get_translation_matrix(-center);

  if( index == 0 )
  {
    this->_transformation = move_to_center;
  }
  else
  {
    this->_transformation.multiply(move_to_center);
  }

  LOG(4, "Do the scaling on the origin");
  this->_transformation.multiply(data.matrix);

  LOG(4, "Move back to its origin");
  move_to_center[2][0] = center.x;
  move_to_center[2][1] = center.y;
  move_to_center[2][2] = center.z;

  this->_transformation.multiply(move_to_center);
}

void Transformation::_scaling_on_coordinate(const TransformationData &data, const unsigned int &index, const Coordinate &center)
{
  LOG(2, "...");
  MatrixForm move_to_center = this->_get_translation_matrix(-data.center);

  if( index == 0 )
  {
    this->_transformation = move_to_center;
  }
  else
  {
    this->_transformation.multiply(move_to_center);
  }

  LOG(4, "Do the scaling on the origin");
  this->_transformation.multiply(data.matrix);

  LOG(4, "Move back to its origin");
  move_to_center[2][0] = center.x;
  move_to_center[2][1] = center.y;
  move_to_center[2][2] = center.z;

  this->_transformation.multiply(move_to_center);
}

void Transformation::_set_rotation_data(const TransformationData &data, const unsigned int &index, const Coordinate &center)
{
  LOG(16, "...");

  switch(data.point)
  {
    case TransformationPoint::ON_WORLD_CENTER:
    {
      this->_rotation_on_world_center(data, index, center);
      break;
    }

    case TransformationPoint::ON_ITS_OWN_CENTER:
    {
      this->_rotation_on_its_own_center(data, index, center);
      break;
    }

    case TransformationPoint::ON_ITS_OWN_AXIS:
    {
      this->_rotation_on_its_own_axis(data, index, center);
      break;
    }

    case TransformationPoint::ON_GIVEN_COORDINATE:
    {
      this->_rotation_on_coordinate(data, index, center);
      break;
    }

    default:
    {
      LOG(1, "");
      LOG(1, "");
      LOG(1, "ERROR! Invalid TransformationPoint used: %d", data.point);
    }
  }
}

void Transformation::_rotation_on_world_center(const TransformationData &data, const unsigned int &index, const Coordinate &center)
{
  // LOG(16, "Just rotate it, as all rotations are based on the world center");
  if( index == 0 )
  {
    this->_transformation = data.matrix;
  }
  else
  {
    this->_transformation.multiply(data.matrix);
  }
}

void Transformation::_rotation_on_its_own_axis(const TransformationData &data, const unsigned int &index, const Coordinate &center)
{
  LOG(2, "...");
  MatrixForm move_to_center = this->_get_translation_matrix(-center);

  if( index == 0 )
  {
    this->_transformation = move_to_center;
  }
  else
  {
    this->_transformation.multiply(move_to_center);
  }

  // https://www.youtube.com/watch?v=gRVxv8kWl0Q
  big_double distance = sqrt( center.y * center.y + center.z * center.z );
  big_double angle_beta = atan( center.x / center.z );
  big_double angle_alfa = atan( center.y / distance );

  LOG(4, "Do the rotation on the object own center");
  this->_transformation.multiply( _get_y_rotation_matrix(-angle_beta) );
  this->_transformation.multiply( _get_x_rotation_matrix(angle_alfa) );
  this->_transformation.multiply( data.matrix );
  this->_transformation.multiply( _get_x_rotation_matrix(-angle_alfa) );
  this->_transformation.multiply( _get_y_rotation_matrix(angle_beta) );

  LOG(4, "Move back to its origin");
  move_to_center[2][0] = center.x;
  move_to_center[2][1] = center.y;
  move_to_center[2][2] = center.z;

  this->_transformation.multiply(move_to_center);
}

void Transformation::_rotation_on_its_own_center(const TransformationData &data, const unsigned int &index, const Coordinate &center)
{
  LOG(2, "...");
  MatrixForm move_to_center = this->_get_translation_matrix(-center);

  if( index == 0 )
  {
    this->_transformation = move_to_center;
  }
  else
  {
    this->_transformation.multiply(move_to_center);
  }

  LOG(4, "Do the rotation on the origin");
  this->_transformation.multiply(data.matrix);

  LOG(4, "Move back to its origin");
  move_to_center[2][0] = center.x;
  move_to_center[2][1] = center.y;
  move_to_center[2][2] = center.z;

  this->_transformation.multiply(move_to_center);
}

void Transformation::_rotation_on_coordinate(const TransformationData &data, const unsigned int &index, const Coordinate &center)
{
  LOG(2, "...");
  MatrixForm move_to_center = this->_get_translation_matrix(-data.center);

  if( index == 0 )
  {
    this->_transformation = move_to_center;
  }
  else
  {
    this->_transformation.multiply(move_to_center);
  }

  LOG(4, "Do the rotation on the origin");
  this->_transformation.multiply(data.matrix);

  LOG(4, "Move back to its origin");
  move_to_center[2][0] = center.x;
  move_to_center[2][1] = center.y;
  move_to_center[2][2] = center.z;

  this->_transformation.multiply(move_to_center);
}

void Transformation::remove_transformation(const std::string name)
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

const std::vector<TransformationData>& Transformation::getTransformations() const
{
  return this->transformations;
}

unsigned int Transformation::size() const
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

  output << "[" << object._transformation << "] ";

  for( auto data : object.transformations )
  {
    output << data;

    if( index != size )
    {
      output << " - ";
    }

    index++;
  }

  return output;
}
