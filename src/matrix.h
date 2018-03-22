#ifndef GTKMM_APP_MATRIX_H
#define GTKMM_APP_MATRIX_H

#include <cassert>
#include <iostream>

#include "array.h"

/**
 * C++ Matrix Class
 * https://stackoverflow.com/questions/2076624/c-matrix-class
 *
 * A proper way to create a matrix in c++
 * https://stackoverflow.com/questions/618511/a-proper-way-to-create-a-matrix-in-c
 *
 * error: incompatible types in assignment of 'long int (*)[4]' to 'long int [4][4]'
 * https://stackoverflow.com/questions/49312484/error-incompatible-types-in-assignment-of-long-int-4-to-long-int
 */
template <unsigned int matrix_width=3, unsigned int matrix_height=3, typename matrix_datatype=long int>
struct Matrix : public Array< matrix_height, Array< matrix_width, matrix_datatype > >
{
  Matrix()
  {
  }

  Matrix(matrix_datatype initial)
  {
    this->clear(initial);
  }

  Matrix(std::initializer_list< std::initializer_list< matrix_datatype > > raw_data)
  {
    // std::cout << raw_data.size() << std::endl;
    assert(raw_data.size() == matrix_height);

    // std::cout << raw_data.begin()->size() << std::endl;
    assert(raw_data.begin()->size() == matrix_width);

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

  void clear(matrix_datatype initial=0)
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

  void multiply(Matrix &matrix)
  {
    unsigned int line;
    unsigned int column;
    unsigned int step;
    matrix_datatype old_matrix[matrix_height][matrix_width];

    for(line = 0; line < matrix_height; line++)
    {
      for(column = 0; column < matrix_width; column++)
      {
        old_matrix [line][column] = this->_data[line][column];
        this->_data[line][column] = 0;
      }
    }

    for(line = 0; line < matrix_height; line++)
    {
      for(column = 0; column < matrix_width; column++)
      {
        for(step = 0; step < matrix_width; step++)
        {
          this->_data[line][column] += old_matrix[line][step] * matrix._data[step][column];
        }
        // std::cout << "this->_data[line][column] = " << this->_data[line][column] << std::endl;
      }
    }
    // If you would like to preserve the original value, it can be returned here
    // return old_matrix;
  }

  /**
   * Prints a more beauty version of the matrix when called on `std::cout<< matrix << std::end;`
   */
  friend std::ostream& operator<<( std::ostream &output, const Matrix &matrix )
  {
    unsigned int line;
    unsigned int column;
    output << "{";

    for( line=0; line < matrix_height; line++ )
    {
      output << "{";

      for( column=0; column < matrix_width; column++ )
      {
        output << matrix._data[line][column];

        if( column != matrix_width-1 )
        {
          output << ", ";
        }
      }

      if( line != matrix_height-1 )
      {
        output << "}, ";
      }
      else
      {
        output << "}";
      }
    }

    output << "}";
    return output;
  }
};

#endif // GTKMM_APP_MATRIX_H
