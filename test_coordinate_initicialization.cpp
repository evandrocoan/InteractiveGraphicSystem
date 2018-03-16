#include <iostream>

#include "src/tinyformat.h"
#include "src/coordinate.h"

/**
 * To build it use:
 *     g++ -std=c++11 test_coordinate_initicialization.cpp -o main
 */

class MyCustomClassType
{
  int var;

  friend std::ostream &operator<<( std::ostream &output, const MyCustomClassType &my_custom_class_type )
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

int main (int argc, char *argv[])
{
  Coordinate coordinate;

  function();
  std::cout << tfm::format("Coordinate: %s", coordinate) << std::endl;
}

