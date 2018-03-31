#include <iostream>

#include "src/tinyformat.h"
#include "src/coordinate.h"

Coordinate _default_coordinate_value_parameter{0.0, 0.0, 1.0};

/**
 * To build it use:
 *     g++ -std=c++11 test_coordinate_initicialization.cpp -o main
 */

struct MyCustomClassType
{
  int var;
  Coordinate coordinate;

  MyCustomClassType(Coordinate center = _default_coordinate_value_parameter) :
      coordinate{center}
  {
  }

  friend std::ostream& operator<<( std::ostream &output, const MyCustomClassType &my_custom_class_type )
  {
    output << "var: " << my_custom_class_type.var << ", " << my_custom_class_type.coordinate;
    return output;
  }
};

// C++11 syntax initialization call to the default constructor
MyCustomClassType _my_custom_class_type{};

void print_my_custom_class_type(MyCustomClassType my_custom_class_type = _my_custom_class_type)
{
  std::cout << "my_custom_class_type: " << my_custom_class_type << std::endl;
}

void print_coordinate(Coordinate coordinate)
{
  std::cout << tfm::format("Coordinate: %s", coordinate) << std::endl;
}

int main (int argc, char* argv[])
{
  std::cout << "Running..." << std::endl;
  MyCustomClassType my_custom_class_type;

  print_my_custom_class_type();
  print_my_custom_class_type(my_custom_class_type);
  std::cout << std::endl;

  Coordinate coordinate;
  print_coordinate(coordinate);

  Coordinate coordinate2{1};
  print_coordinate(coordinate2);
}

