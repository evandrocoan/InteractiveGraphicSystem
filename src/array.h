#ifndef GTKMM_APP_ARRAY_H
#define GTKMM_APP_ARRAY_H

#include <cassert>
#include <iostream>

/**
 * Because you can multiply a `Array` by an `Matrix`, but not a vice-versa.
 */
#include "matrix.h"

/**
 * C++ Matrix Class
 * https://stackoverflow.com/questions/2076624/c-matrix-class
 *
 * error: incompatible types in assignment of 'long int (*)[4]' to 'long int [4][4]'
 * https://stackoverflow.com/questions/49312484/error-incompatible-types-in-assignment-of-long-int-4-to-long-int
 */
template <unsigned int array_width, typename array_datatype=long int>
struct Array
{
  array_datatype _data[array_width];

  Array()
  {
  }

  Array(std::initializer_list< array_datatype > raw_data)
  {
    unsigned int data_size = raw_data.size();
    unsigned int column_index = 0;
    // std::cout << data_size << std::endl;

    if( data_size == 1 )
    {
      array_datatype initial = *(raw_data.begin());

      for( ; column_index < array_width; column_index++ )
      {
        this->_data[column_index] = initial;
      }
    }
    else
    {
      assert(data_size == array_width);

      for( auto column : raw_data )
      {
        this->_data[column_index] = column;
        column_index++;
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
  array_datatype* operator[](int line)
  {
    return this->_data;
  }

  void multiply(Matrix<array_width, array_width, array_datatype> matrix)
  {
    int column;
    int step;

    array_datatype old_array[array_width];

    for(column = 0; column < array_width; column++)
    {
      old_array  [column] = this->_data[column];
      this->_data[column] = 0;
    }

    for(column = 0; column < array_width; column++)
    {
      for(step = 0; step < array_width; step++)
      {
        this->_data[column] += old_array[step] * matrix._data[step][column];
      }
    }
    // If you would like to preserve the original value, it can be returned here
    // return old_array;
  }

  /**
   * Prints a more beauty version of the matrix when called on `std::cout<< matrix << std::end;`
   */
  friend std::ostream& operator<<( std::ostream &output, const Array &matrix )
  {
    unsigned int column;
    output << "{";

    for( column=0; column < array_width; column++ )
    {
      output << matrix._data[column];

      if( column != array_width-1 )
      {
        output << ", ";
      }
    }

    output << "}";
    return output;
  }
};

#endif // GTKMM_APP_ARRAY_H
