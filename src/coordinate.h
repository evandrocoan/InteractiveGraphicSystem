#ifndef GTKMM_APP_COORDINATE_H
#define GTKMM_APP_COORDINATE_H

#include <limits>
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
struct Coordinate : public Array<MATRICES_DIMENSION, big_double>
{
  typedef Array<MATRICES_DIMENSION, big_double> BaseClass;

  // Inheriting constructors
  // https://stackoverflow.com/questions/347358/inheriting-constructors
  using BaseClass::Array;

  Coordinate(big_double x, big_double y, big_double z = 1) :
      Array{x, y, z}
  {
  }

  ~Coordinate()
  {
  }

  big_double getx() const
  {
    return this->_data[0];
  }

  big_double gety() const
  {
    return this->_data[1];
  }

  big_double getz() const
  {
    return this->_data[2];
  }

  /**
   * Data to Object operators.
   */
  Coordinate operator-() const { Coordinate negative_value{*this}; for( unsigned int index = 0; index < MATRICES_DIMENSION; index++ )
      { negative_value._data[index] = -negative_value._data[index]; } return negative_value; }

  Coordinate& operator+=(const big_double& data) { for( unsigned int index = 0; index < MATRICES_DIMENSION; index++ )
      { this->_data[index] += data; } return *this; }

  Coordinate& operator-=(const big_double& data) { for( unsigned int index = 0; index < MATRICES_DIMENSION; index++ )
      { this->_data[index] -= data; } return *this; }

  /**
   * Object to Object operators.
   */
  Coordinate& operator+=(const Array& object) { for( unsigned int index = 0; index < MATRICES_DIMENSION; index++ )
      { this->_data[index] += object._data[index]; } return *this; }

  Coordinate& operator-=(const Array& object) { for( unsigned int index = 0; index < MATRICES_DIMENSION; index++ )
      { this->_data[index] -= object._data[index]; } return *this; }

  /**
   * Double Data to Object precision comparison.
   */
  bool operator!=(const big_double& data) { return !(*this == data); }
  bool operator<=(const big_double& data) { return *this > data;     }
  bool operator>=(const big_double& data) { return *this < data;     }

  bool operator<(const big_double& data) {
  if( *this == data ) { return false; }
  for( unsigned int index = 0; index < MATRICES_DIMENSION; index++ )
    { if( this->_data[index] > data ) { return false; } } return true; }

  bool operator>(const big_double& data) {
    if( *this == data ) { return false; }
    for( unsigned int index = 0; index < MATRICES_DIMENSION; index++ )
      { if( this->_data[index] < data ) { return false; } } return true; }

  /**
   * Comparing doubles
   * https://stackoverflow.com/questions/4010240/comparing-doubles
   *
   * What's a good way to check for ``close enough'' floating-point equality?
   * http://c-faq.com/fp/fpequal.html
   */
  bool operator==(const big_double& data)
  {
    for( unsigned int index = 0; index < MATRICES_DIMENSION; index++ )
    {
      if( this->_data[index] == data
          || std::abs(this->_data[index] - data)
             < std::abs( std::min( this->_data[index], data ) ) * std::numeric_limits< big_double >::epsilon() )
      {
        return false;
      }
    }
    return true;
  }

  /**
   * Double Object to Object precision comparison.
   */
  bool operator!=(const Coordinate& object) { return !(*this == object); }
  bool operator<=(const Coordinate& object) { return *this > object;     }
  bool operator>=(const Coordinate& object) { return *this < object;     }

  bool operator<(const Coordinate& object) {
  if( *this == object ) { return false; }
  for( unsigned int index = 0; index < MATRICES_DIMENSION; index++ )
    { if( this->_data[index] > object._data[index] ) { return false; } } return true; }

  bool operator>(const Coordinate& object) {
    if( *this == object ) { return false; }
    for( unsigned int index = 0; index < MATRICES_DIMENSION; index++ )
      { if( this->_data[index] < object._data[index] ) { return false; } } return true; }

  bool operator==(const Coordinate& object)
  {
    for( unsigned int index = 0; index < MATRICES_DIMENSION; index++ )
    {
      if( this->_data[index] == object._data[index]
          || std::abs(this->_data[index] - object._data[index])
             < std::abs( std::min( this->_data[index], object._data[index] ) ) * std::numeric_limits< big_double >::epsilon() )
      {
        return false;
      }
    }
    return true;
  }

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
