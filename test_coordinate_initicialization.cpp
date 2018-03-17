#include <iostream>

#include "src/tinyformat.h"
#include "src/coordinate.h"

/**
 * To build it use:
 *     g++ -std=c++11 test_coordinate_initicialization.cpp -o main
 */

struct MyCustomClassType
{
  int var;
  Coordinate coordinate;

  MyCustomClassType(Coordinate rotation_center = _default_coordinate_value_parameter) :
      coordinate{rotation_center}
  {
  }

  friend std::ostream& operator<<( std::ostream &output, const MyCustomClassType &my_custom_class_type )
  {
    output << my_custom_class_type.var;
    return output;
  }
};

// C++11 syntax initialization call to the default constructor
MyCustomClassType _my_custom_class_type{};

void function(MyCustomClassType my_custom_class_type = _my_custom_class_type)
{
  std::cout << my_custom_class_type << std::endl;
}

void print_coordinate(Coordinate coordinate)
{
  std::cout << tfm::format("Coordinate:     %s", coordinate) << std::endl;
  std::cout << tfm::format("is_initialized: %s", coordinate.is_initialized) << std::endl;
}

int main (int argc, char *argv[])
{
  MyCustomClassType my_custom_class_type;
  function();
  function(my_custom_class_type);

  std::cout << std::endl;
  Coordinate coordinate;
  print_coordinate(coordinate);

  std::cout << std::endl;
  Coordinate coordinate2{1};
  print_coordinate(coordinate2);
}

