#ifndef GTKMM_APP_TRANSFORMATION
#define GTKMM_APP_TRANSFORMATION

#include "string"
#include "coordinate.h"

enum RotationType
{
  ON_WORLD_CENTER,
  ON_ITS_OWN_CENTER,
  ON_GIVEN_COORDINATE
};

class Transformation
{
  Transformation(std::string name);

public:
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
   * As `scalings` and `rotations` does not contain embed the `transformations` matrices. These
   * values are only required when current object is finally being rotated/scaled. This happens
   * right after the `set_geometric_center()` method is called on.
   */
  std::vector<long int[3][3]> scalings;
  std::vector<long int[3][3]> rotations;
  std::vector<long int[3][3]> transformations;

  /**
   * These values are set after calling `set_geometric_center()`. They will be the values used
   * to transform the object when calling `apply()`.
   */
  long int _scalings[3][3];
  long int _rotations[3][3];
  long int _transformations[3][3];
};

#endif // GTKMM_APP_TRANSFORMATION
