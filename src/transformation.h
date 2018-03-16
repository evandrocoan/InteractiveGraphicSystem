#ifndef GTKMM_APP_TRANSFORMATION
#define GTKMM_APP_TRANSFORMATION

#include <cmath>
#include "string"
#include "coordinate.h"

// How to set default parameter as class object in c++?
// https://stackoverflow.com/questions/12121645/how-to-set-default-parameter-as-class-object-in-c
Coordinate _default_coordinate_value_parameter{};

enum RotationType
{
  ON_WORLD_CENTER,
  ON_ITS_OWN_CENTER,
  ON_GIVEN_COORDINATE
};

/**
 * Ignoring the `translations` matrices, the `main_matrix` is the main operation applied after
 * moving the object to the world center.
 */
struct TransformationData
{
  long int main_matrix[3][3];
  Coordinate rotation_center;

  /**
   * This `rotation_center` default value is null, because it is only used when the object is a
   * rotation which should be performed around some specific coordinate as the world center, instead
   * of the object geometric center.
   */
  TransformationData(long int main_matrix[3][3], Coordinate rotation_center = NULL) :
      main_matrix(main_matrix),
      rotation_center(rotation_center)
  {
  }
};

class Transformation
{
public:
  Transformation(std::string name);

  void add_scaling(double scale);
  void add_rotation(double degrees, RotationType, Coordinate);
  void add_translation(Coordinate move);

  /**
   * Before calling `apply()`, you must ensure you had called `set_geometric_center()` within the
   * current object geometric center. Such call only needs to happen once, per object where this
   * transformation is being used on.
   *
   * Note: After calling `set_geometric_center()`, you cannot create new transformations. If you
   * create new transformations, you must call `set_geometric_center()` before using it again.
   */
  long int[3] apply(long int[3]);
  void        set_geometric_center(Coordinate);

  static long int[3] multiply_1x3_matrix(long int[3], long int[3][3]);
  static long int[3] multiply_3x3_matrix(long int[3], long int[3][3]);

protected:
  std::string name;

  /**
   * As `scalings` and `rotations` does not contain embedded all the `transformations` matrices.
   * These values are only required when current object is finally being rotated/scaled. This
   * happens right after the `set_geometric_center()` method is called on.
   */
  std::vector<TransformationData> transformations;

  /**
   * These values are set after calling `set_geometric_center()`. They will be the values used
   * to transform the object when calling `apply()`.
   */
  long int _transformation[3][3];
};

#endif // GTKMM_APP_TRANSFORMATION
