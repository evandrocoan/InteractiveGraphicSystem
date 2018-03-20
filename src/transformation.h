#ifndef GTKMM_APP_TRANSFORMATION
#define GTKMM_APP_TRANSFORMATION

// M_PI flagged as undeclared identifier
// https://stackoverflow.com/questions/26065359/m-pi-flagged-as-undeclared-identifier
#define _USE_MATH_DEFINES

#include <cmath>
#include <vector>
#include <string>

#include "traits.h"
#include "debugger.h"
#include "coordinate.h"

#include "matrixform.h"
#include "transformationdata.h"

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

  static inline GTKMM_APP_MATRICES_DATATYPE convert_degrees_to_radians(GTKMM_APP_MATRICES_DATATYPE degrees);

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
  void apply(Coordinate&);
  void set_geometric_center(Coordinate&);

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
