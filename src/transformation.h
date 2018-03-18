#ifndef GTKMM_APP_TRANSFORMATION
#define GTKMM_APP_TRANSFORMATION

#include <cmath>
#include <vector>
#include <string>

#include "debugger.h"
#include "coordinate.h"
#include "matrixform.h"

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
      type(type),
      matrix(matrix),
      rotation_type(rotation_type),
      rotation_center{rotation_center}
  {
  }

  /**
   * Prints a basic information of this object when called on `std::cout<< matrix << std::end;`
   */
  friend std::ostream& operator<<( std::ostream &output, const TransformationData &data )
  {
    output << data.name << ", type: ";
    output << data.type << ", ";
    output << data.matrix << ", rotation_type: ";
    output << data.rotation_type << ", ";
    output << data.rotation_center;
    return output;
  }
};

class Transformation
{
public:
  void add_scaling(std::string name, Coordinate scale);
  void add_translation(std::string name, Coordinate movement);

  /**
   * Create and configure correctly a rotation.
   *
   * @param degrees [description]
   * @param type    an enum RotationType valid value
   * @param point   this is a optional value, only required when using RotationType::ON_GIVEN_COORDINATE
   */
  void add_rotation(std::string name,
                    Array<3, long double> degrees,
                    Coordinate coordinate=_default_coordinate_value_parameter,
                    RotationType type=RotationType::ON_WORLD_CENTER);

  unsigned int size();
  void remove_transformation(std::string name);

  std::vector<TransformationData>& getTransformations();
  friend std::ostream& operator<<(std::ostream &output, const Transformation &object);

  /**
   * Before calling `apply()`, you must ensure you had called `set_geometric_center()` within the
   * current object geometric center. Such call only needs to happen once, per object where this
   * transformation is being used on.
   *
   * Note: After calling `set_geometric_center()`, you cannot create new transformations. If you
   * create new transformations, you must call `set_geometric_center()` before using it again.
   */
  void apply(Coordinate*);
  void set_geometric_center(Coordinate);

protected:

  /**
   * The `scalings` and `rotations` does not need to contain embedded all the `transformations`
   * matrices. These extra matrices as the transformation matrices to the world center are only
   * required when current object is finally being rotated/scaled.
   *
   * Therefore, this variable is only set within the minimum required information to build the final
   * transformation matrix. This happens right after the `set_geometric_center()` method is called on.
   */
  std::vector<TransformationData> transformations;

  /**
   * These values are set after calling `set_geometric_center()`. They will be the values used
   * to transform the object when calling `apply()`.
   */
  MatrixForm _transformation;

  void _set_scaling_data      (TransformationData&, unsigned int &index, Coordinate &center);
  void _set_rotation_data     (TransformationData&, unsigned int &index, Coordinate &center);
  void _rotation_on_center    (TransformationData&, unsigned int &index, Coordinate &center);
  void _rotation_on_coordinate(TransformationData&, unsigned int &index, Coordinate &center);
};

#endif // GTKMM_APP_TRANSFORMATION
