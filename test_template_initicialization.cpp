#include <vector>
#include <cassert>
#include <iostream>

/**
 * To build it use:
 *     g++ -std=c++11 test_template_initicialization.cpp -o main
 */
template <unsigned int matrix_width, unsigned int matrix_height, typename matrix_datatype=long int>
struct Matrix
{
  matrix_datatype _data[matrix_height][matrix_width];

  Matrix()
  {
  }

  Matrix(matrix_datatype initial)
  {
    unsigned int line;
    unsigned int column;

    for( line=0; line < matrix_height; line++ )
    {
      for( column=0; column < matrix_width; column++ )
      {
        this->_data[line][column] = initial;
      }
    }
  }

  Matrix(std::initializer_list< std::initializer_list< matrix_datatype > > raw_data)
  {
    // std::cout << raw_data.size() << std::endl;
    assert(raw_data.size() <= matrix_height);

    // std::cout << raw_data.begin()->size() << std::endl;
    assert(raw_data.begin()->size() <= matrix_width);

    unsigned int line_index = 0;
    unsigned int column_index;

    for( auto line : raw_data )
    {
      column_index = 0;

      for( auto column : line )
      {
        this->_data[line_index][column_index] = column;
        column_index++;
      }

      line_index++;
    }
  }

  /**
   * Overloads the `[]` array access operator, allowing you to access this class objects as the
   * where usual `C` arrays.
   *
   * @param  line the current line you want to access
   * @return      a pointer to the current line
   */
  matrix_datatype* operator[](int line)
  {
    return this->_data[line];
  }

  /**
   * Prints a more beauty version of the matrix when called on `std::cout<< matrix << std::end;`
   */
  friend std::ostream &operator<<( std::ostream &output, const Matrix &matrix )
  {
    unsigned int line;
    unsigned int column;

    for( line=0; line < matrix_height; line++ )
    {
      for( column=0; column < matrix_width; column++ )
      {
        output << matrix._data[line][column] << ", ";
      }

      output << "\n";
    }

    return output;
  }
};

/**
 * C++ Matrix Class
 * https://stackoverflow.com/questions/2076624/c-matrix-class
 *
 * error: incompatible types in assignment of 'long int (*)[4]' to 'long int [4][4]'
 * https://stackoverflow.com/questions/49312484/error-incompatible-types-in-assignment-of-long-int-4-to-long-int
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

