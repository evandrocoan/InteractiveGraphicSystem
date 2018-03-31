#ifndef GTKMM_APP_COORDINATE_H
#define GTKMM_APP_COORDINATE_H

#include <iostream>

#include "traits.h"
#include "array.h"

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
   * Overloading operators in derived class
   * https://stackoverflow.com/questions/5679073/overloading-operators-in-derived-class
   *
   * C++ static polymorphism (CRTP) and using typedefs from derived classes
   * https://stackoverflow.com/questions/6006614/c-static-polymorphism-crtp-and-using-typedefs-from-derived-classes
   */
  Coordinate& operator+=(const COORDINATE_TYPE& step)
  {
    for( unsigned int index = 0; index < MATRICES_DIMENSION; index++ )
    {
      this->_data[index] += step;
    }
    return *this;
  }

  Coordinate& operator-=(const COORDINATE_TYPE& step)
  {
    for( unsigned int index = 0; index < MATRICES_DIMENSION; index++ )
    {
      this->_data[index] -= step;
    }
    return *this;
  }

  Coordinate& operator+=(const Array& object)
  {
    for( unsigned int index = 0; index < MATRICES_DIMENSION; index++ )
    {
      this->_data[index] += object._data[index];
    }
    return *this;
  }

  Coordinate& operator-=(const Array& object)
  {
    for( unsigned int index = 0; index < MATRICES_DIMENSION; index++ )
    {
      this->_data[index] -= object._data[index];
    }
    return *this;
  }

  Coordinate operator-() const
  {
    Coordinate negative_value{*this};
    for( unsigned int index = 0; index < MATRICES_DIMENSION; index++ )
    {
      negative_value._data[index] = -negative_value._data[index];
    }
    return negative_value;
  }
};

// How to set default parameter as class object in c++?
// https://stackoverflow.com/questions/12121645/how-to-set-default-parameter-as-class-object-in-c
extern Coordinate _default_coordinate_value_parameter;

#endif // GTKMM_APP_COORDINATE_H
