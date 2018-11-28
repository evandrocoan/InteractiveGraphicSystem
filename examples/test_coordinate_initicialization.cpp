#include <iostream>
#include <typeinfo>
#include <algorithm>

#include "../src/tinyformat.h"
#include "../src/coordinate.h"
#include "../src/matrixform.h"

// #define DOCTEST_CONFIG_DISABLE
#ifndef DOCTEST_CONFIG_DISABLE
  #define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#endif
#include "../src/doctest.h"

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

template<typename Type>
void print_type(Type coordinate) {
  std::cout << tfm::format("%s: %s", typeid(Type).name(), coordinate) << std::endl;
}


TEST_CASE("Testing basic coordinate initialization with a constant value")
{
  Coordinate coordinate{2};
  std::ostringstream contents;

  contents << coordinate;
  CHECK( "(2, 2, 2, 1)" == contents.str() );
}

TEST_CASE("Testing basic coordinate sum by scalar") {
  std::ostringstream contents;
  Coordinate coordinate{1.0};

  Coordinate new_coordinate = coordinate + 10.0;
  std::ostringstream().swap(contents); contents << new_coordinate;
  CHECK( "(11, 11, 11, 11)" == contents.str() );

  std::ostringstream().swap(contents); contents << coordinate;
  CHECK( "(1, 1, 1, 1)" == contents.str() );
}

TEST_CASE("Testing basic coordinate sum and attribution by scalar") {
  std::ostringstream contents;
  Coordinate coordinate{1.0};

  coordinate += 10.0;
  std::ostringstream().swap(contents); contents << coordinate;
  CHECK( "(11, 11, 11, 11)" == contents.str() );
}

TEST_CASE("Testing basic coordinate sum by another coordinate") {
  std::ostringstream contents;
  Coordinate coordinate{1.0};
  Coordinate another_coordinate{2.0};

  Coordinate new_coordinate = coordinate + another_coordinate;
  std::ostringstream().swap(contents); contents << new_coordinate;
  CHECK( "(3, 3, 3, 2)" == contents.str() );

  std::ostringstream().swap(contents); contents << coordinate;
  CHECK( "(1, 1, 1, 1)" == contents.str() );
}

TEST_CASE("Testing basic coordinate sum and attribution by another coordinate") {
  std::ostringstream contents;
  Coordinate coordinate{1.0};
  Coordinate another_coordinate{2.0};

  coordinate += another_coordinate;
  std::ostringstream().swap(contents); contents << coordinate;
  CHECK( "(3, 3, 3, 2)" == contents.str() );
}

TEST_CASE("Testing basic coordinate negative operator") {
  std::ostringstream contents;
  Coordinate coordinate{1.0};

  Coordinate new_coordinate = -coordinate;
  std::ostringstream().swap(contents); contents << new_coordinate;
  CHECK( "(-1, -1, -1, -1)" == contents.str() );

  std::ostringstream().swap(contents); contents << coordinate;
  CHECK( "(1, 1, 1, 1)" == contents.str() );
}

TEST_CASE("Testing basic coordinate difference by scalar") {
  std::ostringstream contents;
  Coordinate coordinate{1.0};

  Coordinate new_coordinate = coordinate - 10.0;
  std::ostringstream().swap(contents); contents << new_coordinate;
  CHECK( "(-9, -9, -9, -9)" == contents.str() );

  std::ostringstream().swap(contents); contents << coordinate;
  CHECK( "(1, 1, 1, 1)" == contents.str() );
}

TEST_CASE("Testing basic coordinate difference and attribution by scalar") {
  std::ostringstream contents;
  Coordinate coordinate{1.0};

  coordinate -= 10.0;
  std::ostringstream().swap(contents); contents << coordinate;
  CHECK( "(-9, -9, -9, -9)" == contents.str() );
}

TEST_CASE("Testing basic coordinate difference by another coordinate") {
  std::ostringstream contents;
  Coordinate coordinate{1.0};
  Coordinate another_coordinate{2.0};

  Coordinate new_coordinate = coordinate - another_coordinate;
  std::ostringstream().swap(contents); contents << new_coordinate;
  CHECK( "(-1, -1, -1, 0)" == contents.str() );

  std::ostringstream().swap(contents); contents << coordinate;
  CHECK( "(1, 1, 1, 1)" == contents.str() );
}

TEST_CASE("Testing basic coordinate difference and attribution by another coordinate") {
  std::ostringstream contents;
  Coordinate coordinate{1.0};
  Coordinate another_coordinate{2.0};

  coordinate -= another_coordinate;
  std::ostringstream().swap(contents); contents << another_coordinate;
  CHECK( "(2, 2, 2, 1)" == contents.str() );

  std::ostringstream().swap(contents); contents << coordinate;
  CHECK( "(-1, -1, -1, 0)" == contents.str() );
}

TEST_CASE("Testing basic coordinate multiplication") {
  std::ostringstream contents;

  Coordinate coordinate1{1};
  Coordinate coordinate2{2};

  coordinate1.multiply(coordinate1);
  std::ostringstream().swap(contents); contents << coordinate1;
  CHECK( "(1, 1, 1, 1)" == contents.str() );

  coordinate1.multiply(coordinate2);
  std::ostringstream().swap(contents); contents << coordinate2;
  CHECK( "(2, 2, 2, 1)" == contents.str() );
}

TEST_CASE("Testing basic coordinate division by scalar") {
  std::ostringstream contents;
  Coordinate coordinate{1.0};

  Coordinate new_coordinate = coordinate / 10.0;
  std::ostringstream().swap(contents); contents << new_coordinate;
  CHECK( "(0.1, 0.1, 0.1, 0.1)" == contents.str() );

  std::ostringstream().swap(contents); contents << coordinate;
  CHECK( "(1, 1, 1, 1)" == contents.str() );

  new_coordinate = coordinate.divide(100.0);
  std::ostringstream().swap(contents); contents << new_coordinate;
  CHECK( "(0.01, 0.01, 0.01, 0.01)" == contents.str() );

  std::ostringstream().swap(contents); contents << coordinate;
  CHECK( "(0.01, 0.01, 0.01, 0.01)" == contents.str() );
}

TEST_CASE("Testing basic array division by scalar") {
  std::ostringstream contents;

  Array<5, double> array{1};
  std::ostringstream().swap(contents); contents << array;
  CHECK( "(1, 1, 1, 1, 1)" == contents.str() );

  Array<5, double> new_array = array / 10.0;
  std::ostringstream().swap(contents); contents << new_array;
  CHECK( "(0.1, 0.1, 0.1, 0.1, 0.1)" == contents.str() );
}

TEST_CASE("Testing basic matrix multiplication") {
  std::ostringstream contents;

  Coordinate coordinate{2};
  MatrixForm matrix{
    {1, 0, 0, 0},
    {0, 1, 0, 0},
    {0, 0, 1, 0},
    {0, 0, 0, 1}
  };

  matrix.multiply(matrix);
  coordinate.multiply(matrix);

  // https://stackoverflow.com/questions/2848087/how-to-clear-stringstream
  std::ostringstream().swap(contents); contents << coordinate;
  CHECK( "(2, 2, 2, 1)" == contents.str() );

  std::ostringstream().swap(contents); contents << matrix;
  CHECK( "{(1, 0, 0, 0), (0, 1, 0, 0), (0, 0, 1, 0), (0, 0, 0, 1)}" == contents.str() );
}


#ifdef DOCTEST_CONFIG_DISABLE
int main (int argc, char* argv[])
{
  std::cout << "Running..." << std::endl;
  MyCustomClassType my_custom_class_type;

  print_my_custom_class_type();
  print_my_custom_class_type(my_custom_class_type);

  std::cout << std::endl << "Basic initialization" << std::endl;
  Coordinate coordinate;
  print_type(coordinate);

  Coordinate coordinate2{1};
  Coordinate coordinate3{2};

  print_type(coordinate2);
  print_type(coordinate3);

  std::cout << std::endl << "Basic coordinate multiplication" << std::endl;
  coordinate2.multiply(coordinate3);
  print_type(coordinate2);

  MatrixForm matrix{
    {1, 0, 0, 0},
    {0, 1, 0, 0},
    {0, 0, 1, 0},
    {0, 0, 0, 1}
  };
  print_type(matrix);

  std::cout << std::endl << "Basic matrix multiplication" << std::endl;
  coordinate3.multiply(matrix);
  print_type(coordinate3);

  matrix.multiply(matrix);
  print_type(matrix);
}
#endif
