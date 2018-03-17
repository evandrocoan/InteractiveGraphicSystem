#ifndef GTKMM_APP_MATRIX_FORM_H
#define GTKMM_APP_MATRIX_FORM_H

#include "matrix.h"

/**
 * Similar to the `Coordinate` class, which extends the `Array<>` template type. This allow you to
 * instantiate the matrices as `MatrixForm` instead of `Matrix<>`, due the C++ templates used.
 *
 * Template default arguments
 * https://stackoverflow.com/questions/15373823/template-default-arguments
 */
#define GTKMM_APP_MATRIX_FORM_H_DATATYPE  long int
#define GTKMM_APP_MATRIX_FORM_H_DIMENSION 3, 3

struct MatrixForm : public Matrix< GTKMM_APP_MATRIX_FORM_H_DIMENSION, GTKMM_APP_MATRIX_FORM_H_DATATYPE >
{
  // Inheriting constructors
  // https://stackoverflow.com/questions/347358/inheriting-constructors
  using Matrix< GTKMM_APP_MATRIX_FORM_H_DIMENSION, GTKMM_APP_MATRIX_FORM_H_DATATYPE >::Matrix;
};

// How to set default parameter as class object in c++?
// https://stackoverflow.com/questions/12121645/how-to-set-default-parameter-as-class-object-in-c
extern MatrixForm _default_matrix_value_parameter;

#endif // GTKMM_APP_MATRIX_FORM_H
