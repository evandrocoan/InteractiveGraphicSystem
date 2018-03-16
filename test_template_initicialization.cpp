
#include <iostream>

#include "src/matrix.h"

/**
 * To build it use:
 *     g++ -std=c++11 test_template_initicialization.cpp -o main
 */
int main (int argc, char *argv[])
{
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

