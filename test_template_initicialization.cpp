#include <vector>
#include <cassert>
#include <iostream>

/**
 * To build it use:
 *     g++ -std=c++11 test_template_initicialization.cpp -o main
 */
template <int width, int height>
struct Matrix
{
  long int _data[height][width];

  Matrix()
  {
  }

  Matrix(std::initializer_list<std::initializer_list<long int> > raw_data)
  {
    auto index = raw_data.begin();
    std::vector< std::vector<long int> > input_data;

    std::cout << raw_data.size() << std::endl;
    assert(raw_data.size() <= height);

    std::cout << index->size() << std::endl;
    assert(index->size() <= height);

    for( ; index != raw_data.end(); index++ )
    {
      input_data.push_back( std::vector<long int>(*index) );
    }

    for( unsigned int line = 0; line < height; line++ )
    {
      for( unsigned int column = 0; column < width; column++ )
      {
        this->_data[line][column] = input_data[line][column];
      }
    }
  }

  /**
   * Overloads the `[]` array access operator, allowing you to access this class objects as the
   * where usual `C` arrays.
   *
   * @param  line the current line you want to access
   * @return      a pointer to the current line
   */
  long int* operator[](int line)
  {
    return this->_data[line];
  }

  /**
   * Prints a more beauty version of the matrix when called on `std::cout<< matrix << std::end;`
   */
  friend std::ostream &operator<<( std::ostream &output, const Matrix &matrix )
  {
    int i, j;

    for( i=0; i < height; i++ )
    {
      for( j=0; j < width; j++ )
      {
        output << matrix._data[i][j] << ", ";
      }

      output << matrix._data[i][j] << "\n";
    }
    return output;
  }
};

/**
 * C++ Matrix Class
 * https://stackoverflow.com/questions/2076624/c-matrix-class
 */
int main (int argc, char *argv[])
{
  Matrix<3, 3> matrix;
  std::cout << matrix << std::endl;

  // matrix[0][0] = 911;
  // std::cout << matrix << std::endl;

  // std::cout << matrix[0] << std::endl;
  // std::cout << matrix[0][0] << std::endl;

  Matrix<3,3> matrix2{ {0,0,0}, {0,0,0}, {0,0,0} };
  std::cout << matrix2 << std::endl;
}

