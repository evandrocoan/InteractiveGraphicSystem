#ifndef GTKMM_APP_COORDINATE_H
#define GTKMM_APP_COORDINATE_H

/**
 * 'fabs' : ambiguous call to overloaded function when using templates
 * https://stackoverflow.com/questions/10744451/fabs-ambiguous-call-to-overloaded-function-when-using-templates
 */
#include <cmath>

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

  /**
   * C++ member variable aliases?
   * https://stackoverflow.com/questions/494597/c-member-variable-aliases
   *
   * Memory allocation for references
   * https://stackoverflow.com/questions/11661266/memory-allocation-for-references
   *
   * Does reference variable occupy memory?
   * https://stackoverflow.com/questions/29322688/does-reference-variable-occupy-memory
   */
  big_double& x;
  big_double& y;
  big_double& z;

  Coordinate() :
      Array{},
      x{this->_data[0]},
      y{this->_data[1]},
      z{this->_data[2]}
  {
  }

  Coordinate(big_double initial) :
      Array{initial},
      x{this->_data[0]},
      y{this->_data[1]},
      z{this->_data[2]}
  {
  }

  Coordinate(big_double x, big_double y, big_double z = 1.0) :
      Array{x, y, z},
      x{this->_data[0]},
      y{this->_data[1]},
      z{this->_data[2]}
  {
  }

  Coordinate(const Coordinate& object) :
      Array{object},
      x{this->_data[0]},
      y{this->_data[1]},
      z{this->_data[2]}
  {
  }

  Coordinate& operator=(const Coordinate& object)
  {
    Array::operator=(object);
    this->x = this->_data[0];
    this->y = this->_data[1];
    this->z = this->_data[2];
    return *this;
  }

  ~Coordinate()
  {
  }

  /**
   * Data to Object operators.
   */
  Coordinate operator-() const { Coordinate new_value{*this}; for( unsigned int index = 0; index < MATRICES_DIMENSION; index++ )
      { new_value._data[index] = -new_value._data[index]; } return new_value; }

  Coordinate operator+(const big_double& data) { Coordinate new_value{*this};
      for( unsigned int index = 0; index < MATRICES_DIMENSION; index++ )
      { new_value._data[index] += data; } return new_value; }

  Coordinate operator-(const big_double& data) { Coordinate new_value{*this};
      for( unsigned int index = 0; index < MATRICES_DIMENSION; index++ )
      { new_value._data[index] -= data; } return new_value; }

  Coordinate& operator+=(const big_double& data) { for( unsigned int index = 0; index < MATRICES_DIMENSION; index++ )
      { this->_data[index] += data; } return *this; }

  Coordinate& operator-=(const big_double& data) { for( unsigned int index = 0; index < MATRICES_DIMENSION; index++ )
      { this->_data[index] -= data; } return *this; }

  /**
   * Object to Object operators.
   */
  Coordinate operator+(const Array& object) { Coordinate new_value{*this};
      for( unsigned int index = 0; index < MATRICES_DIMENSION; index++ )
      { new_value._data[index] += object._data[index]; } return new_value; }

  Coordinate operator-(const Array& object) { Coordinate new_value{*this};
      for( unsigned int index = 0; index < MATRICES_DIMENSION; index++ )
      { new_value._data[index] -= object._data[index]; } return new_value; }

  Coordinate& operator+=(const Array& object) { for( unsigned int index = 0; index < MATRICES_DIMENSION; index++ )
      { this->_data[index] += object._data[index]; } return *this; }

  Coordinate& operator-=(const Array& object) { for( unsigned int index = 0; index < MATRICES_DIMENSION; index++ )
      { this->_data[index] -= object._data[index]; } return *this; }

  /**
   * Double Data to Object precision comparison.
   */
  bool operator!=(const big_double& data) const { return !(*this == data); }
  bool operator<=(const big_double& data) const { return *this > data;     }
  bool operator>=(const big_double& data) const { return *this < data;     }

  bool operator<(const big_double& data) const {
  if( *this == data ) { return false; }
  for( unsigned int index = 0; index < MATRICES_DIMENSION; index++ )
    { if( this->_data[index] > data ) { return false; } } return true; }

  bool operator>(const big_double& data) const {
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
  bool operator==(const big_double& data) const
  {
    for( unsigned int index = 0; index < MATRICES_DIMENSION; index++ )
    {
      if( this->_data[index] == data
          || std::fabs(this->_data[index] - data)
             < std::fabs( std::min( this->_data[index], data ) ) * std::numeric_limits< big_double >::epsilon() )
      {
        return false;
      }
    }
    return true;
  }

  /**
   * Double Object to Object precision comparison.
   */
  bool operator!=(const Coordinate& object) const { return !(*this == object); }
  bool operator<=(const Coordinate& object) const { return *this > object;     }
  bool operator>=(const Coordinate& object) const { return *this < object;     }

  bool operator<(const Coordinate& object) const {
  if( *this == object ) { return false; }
  for( unsigned int index = 0; index < MATRICES_DIMENSION; index++ )
    { if( this->_data[index] > object._data[index] ) { return false; } } return true; }

  bool operator>(const Coordinate& object) const {
    if( *this == object ) { return false; }
    for( unsigned int index = 0; index < MATRICES_DIMENSION; index++ )
      { if( this->_data[index] < object._data[index] ) { return false; } } return true; }

  bool operator==(const Coordinate& object) const
  {
    for( unsigned int index = 0; index < MATRICES_DIMENSION; index++ )
    {
      if( this->_data[index] == object._data[index]
          || std::fabs(this->_data[index] - object._data[index])
             < std::fabs( std::min( this->_data[index], object._data[index] ) ) * std::numeric_limits< big_double >::epsilon() )
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
