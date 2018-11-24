#ifndef GTKMM_APP_TRANSFORMATION
#define GTKMM_APP_TRANSFORMATION

// M_PI flagged as undeclared identifier
// https://stackoverflow.com/questions/26065359/m-pi-flagged-as-undeclared-identifier
#define _USE_MATH_DEFINES

#include <cmath>
#include <vector>

#include <string>
#include <iostream>

#include "traits.h"
#include "debugger.h"
#include "utilities.h"

#include "coordinate.h"
#include "matrixform.h"

enum Projection
{
  PARALLEL,
  PERSPECTIVE
};

enum TransformationPoint
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

std::ostream& operator<<(std::ostream &output, const TransformationPoint object);
std::ostream& operator<<(std::ostream &output, const TransformationType object);

/**
 * Ignoring the `translations` matrices, the `matrix` is the main operation applied after
 * moving the object to the world center.
 */
struct TransformationData
{
  std::string name;
  MatrixForm matrix;
  TransformationType type;

  Coordinate center;
  TransformationPoint point;

  /**
   * This `center` default value is null, because it is only used when the object is a
   * rotation which should be performed around some specific coordinate as the world center, instead
   * of the object geometric center.
   */
  TransformationData(std::string name, MatrixForm matrix, TransformationType type,
      TransformationPoint point = TransformationPoint::ON_WORLD_CENTER,
      Coordinate center = _origin_coordinate_value);

  friend std::ostream& operator<<(std::ostream &output, const TransformationData &object);
};

class Transformation
{
public:
  Transformation();
  ~Transformation();

  big_double projectionDistance;
  Transformation* posTransformation;

  /**
   * Create and configure correctly a rotation.
   *
   * @param degrees [description]
   * @param type    an enum TransformationPoint valid value
   * @param point   this is a optional value, only required when using TransformationPoint::ON_GIVEN_COORDINATE
   */
  void add_translation(const std::string name, const Coordinate movement);

  void add_scaling(const std::string name,
                   const Coordinate scale,
                   const TransformationPoint point=TransformationPoint::ON_WORLD_CENTER,
                   const Coordinate center=_origin_coordinate_value);

  void add_rotation(const std::string name,
                    const Coordinate degrees,
                    const TransformationPoint type=TransformationPoint::ON_WORLD_CENTER,
                    const Coordinate point=_origin_coordinate_value);

  void add_matrix(const std::string name,
                  const MatrixForm matrix,
                  const TransformationType type=TransformationType::TRANSLATION,
                  const TransformationPoint point=TransformationPoint::ON_WORLD_CENTER,
                  const Coordinate center=_origin_coordinate_value);

  /**
   * Remove all transformations from this container and set it as uninitialized. So, calling it
   * to convert some point will take no effect, i.e., destroy your data.
   */
  void clear();
  unsigned int size() const;

  void remove_transformation(const std::string name);

  const std::vector<TransformationData>& getTransformations() const;
  friend std::ostream& operator<<(std::ostream &output, const Transformation &object);

  /**
   * Before calling `apply()`, you must ensure you had called `set_geometric_center()` within the
   * current object geometric center. Such call only needs to happen once, per object where this
   * transformation is being used on.
   *
   * Note: After calling `set_geometric_center()`, you cannot create new transformations. If you
   * create new transformations, you must call `set_geometric_center()` before using it again.
   */
  void apply(Coordinate&) const;
  void set_geometric_center(const Coordinate& center);

protected:

  /**
   * The `scalings` and `rotations` does not need to contain embedded all the `transformations`
   * matrices. These extra matrices as the transformation matrices to the world center are only
   * required when current object is finally being rotated/scaled.
   *
   * Therefore, this variable is only set within the minimum required information to build the final
   * transformation matrix. This happens right after the `set_geometric_center()` method is called on.
   */
  std::vector<TransformationData> _transformations;

  /**
   * These values are set after calling `set_geometric_center()`. They will be the values used
   * to transform the object when calling `apply()`.
   */
  bool _isInitialized;
  MatrixForm _transformation;

  /**
   * Accordingly creates the translation, scaling and rotation matrices, given their parameters.
   */
  const MatrixForm _get_translation_matrix(const Coordinate& moves) const;
  const MatrixForm _get_scaling_matrix(const Coordinate& factors) const;
  const MatrixForm _get_x_rotation_matrix(const big_double& degrees, const bool& is_radians=false) const;
  const MatrixForm _get_y_rotation_matrix(const big_double& degrees, const bool& is_radians=false) const;
  const MatrixForm _get_z_rotation_matrix(const big_double& degrees, const bool& is_radians=false) const;

  void _set_translation_data(const TransformationData&, const unsigned int &index, const Coordinate &center);
  void _set_scaling_data    (const TransformationData&, const unsigned int &index, const Coordinate &center);
  void _set_rotation_data   (const TransformationData&, const unsigned int &index, const Coordinate &center);

  void _scaling_on_coordinate    (const TransformationData&, const unsigned int &index, const Coordinate &center);
  void _scaling_on_world_center  (const TransformationData&, const unsigned int &index, const Coordinate &center);
  void _scaling_on_its_own_center(const TransformationData&, const unsigned int &index, const Coordinate &center);

  void _rotation_on_coordinate    (const TransformationData&, const unsigned int &index, const Coordinate &center);
  void _rotation_on_world_center  (const TransformationData&, const unsigned int &index, const Coordinate &center);
  void _rotation_on_its_own_center(const TransformationData&, const unsigned int &index, const Coordinate &center);
};

#endif // GTKMM_APP_TRANSFORMATION
