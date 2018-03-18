
#include <iostream>

#include "src/array.h"
#include "src/matrix.h"
#include "src/matrixform.h"

void matrix_tests();
void matrix_form_tests();
void array_tests();


/**
 * To build it use:
 *     g++ -std=c++11 test_template_initicialization.cpp -o main
 */
int main (int argc, char* argv[])
{
  matrix_tests();

  std::cout << std::endl;
  matrix_form_tests();

  std::cout << std::endl;
  array_tests();
}

void array_tests()
{
  std::cout << "Array tests" << std::endl;
  Array<3, long int> array;

  std::cout << array << std::endl;
  std::cout << array[0] << std::endl;

  Array<3> array2 = {0,0,0};
  std::cout << "array2: " << array2 << std::endl;

  Array<3> array3 = {3};
  std::cout << "array3: " << array3 << std::endl;
}

void matrix_form_tests()
{
  std::cout << "MatrixForm tests" << std::endl;
  MatrixForm matrix;
  std::cout << matrix << std::endl;

  MatrixForm matrix1{{1, 0, 0}, {0, 1, 0}, {100, 100, 100}};
  MatrixForm matrix2{{1, 0, 0}, {0, 1, 0}, {200, 100, 100}};
  std::cout << matrix1 << std::endl;
  std::cout << matrix2 << std::endl;

  matrix1.multiply(matrix2);
  std::cout << matrix1 << std::endl;
  std::cout << matrix2 << std::endl;
}

void matrix_tests()
{
  std::cout << "Matrix tests" << std::endl;
  Matrix<3, 3, long int> matrix;
  std::cout << matrix << std::endl;

  matrix[0][0] = 911;
  std::cout << matrix << std::endl;

  std::cout << matrix[0] << std::endl;
  std::cout << matrix[0][0] << std::endl;

  Matrix<3, 3> matrix2{ {0,0,0}, {0,0,0}, {0,0,0} };
  std::cout << matrix2 << std::endl;

  Matrix<3, 3> matrix3 = { 3 };
  std::cout << matrix3 << std::endl;

  Matrix<3, 1, long int> matrix4 = { 4 };
  std::cout << matrix4 << std::endl;
}

