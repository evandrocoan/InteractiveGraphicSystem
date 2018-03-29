#ifndef GTKMM_APP_TRANSFORMATION_DATA
#define GTKMM_APP_TRANSFORMATION_DATA

#include <string>
#include <iostream>

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

std::ostream& operator<<(std::ostream &output, const RotationType object);
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

  Coordinate rotation_center;
  RotationType rotation_type;

  /**
   * This `rotation_center` default value is null, because it is only used when the object is a
   * rotation which should be performed around some specific coordinate as the world center, instead
   * of the object geometric center.
   */
  TransformationData(std::string name, MatrixForm matrix, TransformationType type,
      RotationType rotation_type = RotationType::ON_WORLD_CENTER,
      Coordinate rotation_center = _default_coordinate_value_parameter);

  friend std::ostream& operator<<(std::ostream &output, const TransformationData &object);
};

#endif // GTKMM_APP_TRANSFORMATION_DATA
