#include "transformationdata.h"

TransformationData::TransformationData(std::string name, MatrixForm matrix, TransformationType type,
        TransformationPoint point, Coordinate center) :
    name(name),
    matrix(matrix),
    type(type),
    center{center},
    point(point)
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
  output << data.point << ", ";
  output << data.center;
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

inline std::ostream& operator<<(std::ostream &output, const TransformationPoint object)
{
  switch (object)
  {
    case TransformationPoint::ON_WORLD_CENTER:     output << "ON_WORLD_CENTER"; break;
    case TransformationPoint::ON_ITS_OWN_CENTER:   output << "ON_ITS_OWN_CENTER"; break;
    case TransformationPoint::ON_GIVEN_COORDINATE: output << "ON_GIVEN_COORDINATE"; break;
    default:
      output << (int) object; break;
  }
  return output;
}
