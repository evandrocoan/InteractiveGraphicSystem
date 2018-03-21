#ifndef GTKMM_APP_TRANSFORMATION_DATA
#define GTKMM_APP_TRANSFORMATION_DATA

enum RotationType
{
  ON_WORLD_CENTER,
  ON_ITS_OWN_CENTER,
  ON_GIVEN_COORDINATE
};

enum TransformationType
{
  SCALING,
  ROTATION,
  TRANSLATION
};

inline std::ostream & operator<<(std::ostream &output, TransformationType object)
{
  switch (object)
  {
    case TransformationType::SCALING:     output << "TransformationType::SCALING"; break;
    case TransformationType::ROTATION:    output << "TransformationType::ROTATION"; break;
    case TransformationType::TRANSLATION: output << "TransformationType::TRANSLATION"; break;
    default:
      output << (int) object; break;
  }
  return output;
}

inline std::ostream & operator<<(std::ostream &output, RotationType object)
{
  switch (object)
  {
    case RotationType::ON_WORLD_CENTER:     output << "RotationType::ON_WORLD_CENTER"; break;
    case RotationType::ON_ITS_OWN_CENTER:   output << "RotationType::ON_ITS_OWN_CENTER"; break;
    case RotationType::ON_GIVEN_COORDINATE: output << "RotationType::ON_GIVEN_COORDINATE"; break;
    default:
      output << (int) object; break;
  }
  return output;
}

/**
 * Ignoring the `translations` matrices, the `matrix` is the main operation applied after
 * moving the object to the world center.
 */
struct TransformationData
{
  std::string name;
  MatrixForm matrix;
  TransformationType type;

  Coordinate rotation_center;
  RotationType rotation_type;

  /**
   * This `rotation_center` default value is null, because it is only used when the object is a
   * rotation which should be performed around some specific coordinate as the world center, instead
   * of the object geometric center.
   */
  TransformationData(std::string name, MatrixForm matrix, TransformationType type,
          RotationType rotation_type = RotationType::ON_WORLD_CENTER,
          Coordinate rotation_center = _default_coordinate_value_parameter) :
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
  friend std::ostream& operator<<( std::ostream &output, const TransformationData &data )
  {
    output << data.name << ", ";
    output << data.type << ", ";
    output << data.matrix << ", ";
    output << data.rotation_type << ", ";
    output << data.rotation_center;
    return output;
  }
};

#endif // GTKMM_APP_TRANSFORMATION_DATA
