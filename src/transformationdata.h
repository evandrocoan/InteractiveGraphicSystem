#ifndef GTKMM_APP_TRANSFORMATION_DATA
#define GTKMM_APP_TRANSFORMATION_DATA

#include <string>
#include <iostream>

#include "coordinate.h"
#include "matrixform.h"

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

#endif // GTKMM_APP_TRANSFORMATION_DATA
