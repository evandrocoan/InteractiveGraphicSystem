#include "transformationdata.h"

TransformationData::TransformationData(std::string name, MatrixForm matrix, TransformationType type,
        RotationType rotation_type, Coordinate rotation_center) :
    name(name),
    matrix(matrix),
    type(type),
    rotation_center{rotation_center},
    rotation_type(rotation_type)
{
}

/**
 * Prints a basic information of this object when called on `std::cout<< matrix << std::end;`
 */
std::ostream& operator<<( std::ostream &output, const TransformationData &data )
{
  output << data.name << ", ";
  output << data.type << ", ";
  output << data.matrix << ", ";
  output << data.rotation_type << ", ";
  output << data.rotation_center;
  return output;
}

inline std::ostream& operator<<(std::ostream &output, const TransformationType object)
{
  switch (object)
  {
    case TransformationType::SCALING:     output << "SCALING"; break;
    case TransformationType::ROTATION:    output << "ROTATION"; break;
    case TransformationType::TRANSLATION: output << "TRANSLATION"; break;
    default:
      output << (int) object; break;
  }
  return output;
}

inline std::ostream& operator<<(std::ostream &output, const RotationType object)
{
  switch (object)
  {
    case RotationType::ON_WORLD_CENTER:     output << "ON_WORLD_CENTER"; break;
    case RotationType::ON_ITS_OWN_CENTER:   output << "ON_ITS_OWN_CENTER"; break;
    case RotationType::ON_GIVEN_COORDINATE: output << "ON_GIVEN_COORDINATE"; break;
    default:
      output << (int) object; break;
  }
  return output;
}
