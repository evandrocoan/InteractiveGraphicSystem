#ifndef GTKMM_APP_COORDINATE_H
#define GTKMM_APP_COORDINATE_H

#include <iostream>

#include "traits.h"
#include "array.h"

/**
 * Overloading operators in derived class
 * https://stackoverflow.com/questions/5679073/overloading-operators-in-derived-class
 *
 * C++ static polymorphism (CRTP) and using typedefs from derived classes
 * https://stackoverflow.com/questions/6006614/c-static-polymorphism-crtp-and-using-typedefs-from-derived-classes
 */
struct Coordinate : public Array<MATRICES_DIMENSION, COORDINATE_TYPE>
{
  typedef Array<MATRICES_DIMENSION, COORDINATE_TYPE> BaseClass;

  // Inheriting constructors
  // https://stackoverflow.com/questions/347358/inheriting-constructors
  using BaseClass::Array;

  Coordinate(COORDINATE_TYPE x, COORDINATE_TYPE y, COORDINATE_TYPE z = 1) :
      Array{x, y, z}
  {
  }

  ~Coordinate()
  {
  }

  COORDINATE_TYPE getx() const
  {
    return this->_data[0];
  }

  COORDINATE_TYPE gety() const
  {
    return this->_data[1];
  }

  COORDINATE_TYPE getz() const
  {
    return this->_data[2];
  }

  /**
   * Data to Object operators.
   */
  Coordinate operator-() const { Coordinate negative_value{*this}; for( unsigned int index = 0; index < MATRICES_DIMENSION; index++ )
      { negative_value._data[index] = -negative_value._data[index]; } return negative_value; }

  Coordinate& operator+=(const COORDINATE_TYPE& data) { for( unsigned int index = 0; index < MATRICES_DIMENSION; index++ )
      { this->_data[index] += data; } return *this; }

  Coordinate& operator-=(const COORDINATE_TYPE& data) { for( unsigned int index = 0; index < MATRICES_DIMENSION; index++ )
      { this->_data[index] -= data; } return *this; }

  /**
   * Object to Object operators.
   */
  Coordinate& operator+=(const Array& object) { for( unsigned int index = 0; index < MATRICES_DIMENSION; index++ )
      { this->_data[index] += object._data[index]; } return *this; }

  Coordinate& operator-=(const Array& object) { for( unsigned int index = 0; index < MATRICES_DIMENSION; index++ )
      { this->_data[index] -= object._data[index]; } return *this; }

  /**
   * Compute the 1/x for all values on the coordinate.
   * @return a new Coordinate object copy with the changed values
   */
  Coordinate inverse() const
  {
    Coordinate inverse_value{*this};
    for( unsigned int index = 0; index < MATRICES_DIMENSION; index++ )
    {
      if( inverse_value._data[index] != 0 )
      {
        inverse_value[index] = 1.0 / inverse_value._data[index];
      }
      else
      {
        inverse_value[index] = 0.0;
      }
    }
    return inverse_value;
  }
};

// How to set default parameter as class object in c++?
// https://stackoverflow.com/questions/12121645/how-to-set-default-parameter-as-class-object-in-c
extern Coordinate _default_coordinate_value_parameter;

#endif // GTKMM_APP_COORDINATE_H
