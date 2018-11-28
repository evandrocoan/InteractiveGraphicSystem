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
struct Coordinate : public Array<MATRICES_DIMENSION, big_double, Coordinate>
{
  typedef Array< MATRICES_DIMENSION, big_double, Coordinate > SuperClass;

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
  big_double& w;

  Coordinate() :
      SuperClass{},
      x{this->_data[0]},
      y{this->_data[1]},
      z{this->_data[2]},
      w{this->_data[3]}
  {
    this->w = 1.0;
  }

  Coordinate(big_double initial) :
      SuperClass{initial},
      x{this->_data[0]},
      y{this->_data[1]},
      z{this->_data[2]},
      w{this->_data[3]}
  {
    this->w = 1.0;
  }

  Coordinate(big_double x, big_double y, big_double z) :
      SuperClass{x, y, z, 1.0},
      x{this->_data[0]},
      y{this->_data[1]},
      z{this->_data[2]},
      w{this->_data[3]}
  {
  }

  Coordinate(const Coordinate& object) :
      SuperClass{object},
      x{this->_data[0]},
      y{this->_data[1]},
      z{this->_data[2]},
      w{this->_data[3]}
  {
  }

  Coordinate& operator=(const Coordinate& object)
  {
    SuperClass::operator=(object);
    this->x = this->_data[0];
    this->y = this->_data[1];
    this->z = this->_data[2];
    this->w = this->_data[3];
    return *this;
  }

  ~Coordinate()
  {
  }

  /**
   * Data to Object operators.
   *
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
   * Object to Object precision comparison.
   */
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
};

// How to set default parameter as class object in c++?
// https://stackoverflow.com/questions/12121645/how-to-set-default-parameter-as-class-object-in-c
extern Coordinate _origin_coordinate_value;

#endif // GTKMM_APP_COORDINATE_H
