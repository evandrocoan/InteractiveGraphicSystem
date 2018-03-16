#ifndef GTKMM_APP_COORDINATE
#define GTKMM_APP_COORDINATE

#include <iostream>

struct Coordinate
{
  Coordinate() :
      is_initialized(false)
  {
  }

  Coordinate(long int x, long int y, long int z = 1) :
      coordinates{x, y, z},
      is_initialized(true)
  {
  }

  ~Coordinate()
  {
  }

  friend std::ostream &operator<<( std::ostream &output, const Coordinate &coordinate )
  {
    output << coordinate.is_initialized
           << "(" << coordinate.getx()
           << ", " << coordinate.gety()
           << ", " << coordinate.getz()
           << ")";
    return output;
  }

  long int getx() const
  {
    return this->coordinates[0];
  }

  long int gety() const
  {
    return this->coordinates[1];
  }

  long int getz() const
  {
    return this->coordinates[2];
  }

  bool is_initialized;
  long int coordinates[3];
};

// How to set default parameter as class object in c++?
// https://stackoverflow.com/questions/12121645/how-to-set-default-parameter-as-class-object-in-c
Coordinate _default_coordinate_value_parameter{};

#endif // GTKMM_APP_Coordinate
