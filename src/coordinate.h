#ifndef GTKMM_APP_COORDINATE_H
#define GTKMM_APP_COORDINATE_H

#include <iostream>

#include "traits.h"
#include "array.h"

struct Coordinate : public Array<GTKMM_APP_MATRICES_DIMENSION, GTKMM_APP_MATRICES_DATATYPE>
{
  // Inheriting constructors
  // https://stackoverflow.com/questions/347358/inheriting-constructors
  using Array<GTKMM_APP_MATRICES_DIMENSION, GTKMM_APP_MATRICES_DATATYPE>::Array;

  Coordinate(GTKMM_APP_MATRICES_DATATYPE x, GTKMM_APP_MATRICES_DATATYPE y, GTKMM_APP_MATRICES_DATATYPE z = 1) :
      Array{x, y, z}
  {
  }

  ~Coordinate()
  {
  }

  GTKMM_APP_MATRICES_DATATYPE getx() const
  {
    return this->_data[0];
  }

  GTKMM_APP_MATRICES_DATATYPE gety() const
  {
    return this->_data[1];
  }

  GTKMM_APP_MATRICES_DATATYPE getz() const
  {
    return this->_data[2];
  }
};

// How to set default parameter as class object in c++?
// https://stackoverflow.com/questions/12121645/how-to-set-default-parameter-as-class-object-in-c
extern Coordinate _default_coordinate_value_parameter;

#endif // GTKMM_APP_COORDINATE_H
