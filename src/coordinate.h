#ifndef GTKMM_APP_COORDINATE_H
#define GTKMM_APP_COORDINATE_H

#include <iostream>
#include "array.h"

/**
 * Template default arguments
 * https://stackoverflow.com/questions/15373823/template-default-arguments
 */
#define GTKMM_APP_COORDINATE_H_DATATYPE long int

struct Coordinate : public Array<3, GTKMM_APP_COORDINATE_H_DATATYPE>
{
  Coordinate() :
      Array{},
      is_initialized(false)
  {
  }

  Coordinate(std::initializer_list< GTKMM_APP_COORDINATE_H_DATATYPE > raw_data) :
      Array(raw_data),
      is_initialized(true)
  {
  }

  Coordinate(GTKMM_APP_COORDINATE_H_DATATYPE x, GTKMM_APP_COORDINATE_H_DATATYPE y, GTKMM_APP_COORDINATE_H_DATATYPE z = 1) :
      Array{x, y, z},
      is_initialized(true)
  {
  }

  ~Coordinate()
  {
  }

  GTKMM_APP_COORDINATE_H_DATATYPE getx() const
  {
    return this->_data[0];
  }

  GTKMM_APP_COORDINATE_H_DATATYPE gety() const
  {
    return this->_data[1];
  }

  GTKMM_APP_COORDINATE_H_DATATYPE getz() const
  {
    return this->_data[2];
  }

  bool is_initialized;
};

// How to set default parameter as class object in c++?
// https://stackoverflow.com/questions/12121645/how-to-set-default-parameter-as-class-object-in-c
Coordinate _default_coordinate_value_parameter{};

#endif // GTKMM_APP_COORDINATE_H
