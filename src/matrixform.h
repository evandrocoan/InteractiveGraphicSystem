#ifndef GTKMM_APP_MATRIX_FORM_H
#define GTKMM_APP_MATRIX_FORM_H

/**
 * Similar to the `Coordinate` class, which extends the `Array<>` template type. This allow you to
 * instantiate the matrices as `MatrixForm` instead of `Matrix<>`, due the C++ templates used.
 *
 * Template default arguments
 * https://stackoverflow.com/questions/15373823/template-default-arguments
 */
#include "matrix.h"
#include "traits.h"

struct MatrixForm : public Matrix<MATRICES_DIMENSION, MATRICES_DIMENSION, big_double>
{
  // Inheriting constructors
  // https://stackoverflow.com/questions/347358/inheriting-constructors
  using Matrix< MATRICES_DIMENSION, MATRICES_DIMENSION, big_double >::Matrix;
};

// How to set default parameter as class object in c++?
// https://stackoverflow.com/questions/12121645/how-to-set-default-parameter-as-class-object-in-c
extern MatrixForm _default_matrix_value_parameter;

#endif // GTKMM_APP_MATRIX_FORM_H
