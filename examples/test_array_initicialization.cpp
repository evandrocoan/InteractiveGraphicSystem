#include <iostream>
#include <array>

/**
 * To build it use:
 *     g++ -std=c++11 test_array_initicialization.cpp -o test_array_initicialization
 */

class Transformation
{
public:
  Transformation() {};
  // Transformation() : transformation_matrix() {};
  long int transformation_matrix[3][3];
};

void transformation_example()
{
  Transformation t;
  int i, j;

  for( i=0; i < 3; i++ )
  {
    for( j=0; j < 3; j++ )
    {
      std::cout << t.transformation_matrix[i][j] << std::endl;
    }
  }
}

void array_alias_example()
{
  std::array<int, 5> my_array = {1, 2, 3, 4, 5};
  int& a1 = my_array[0];

  std::cout << "a1: " << a1 << std::endl;

  my_array[0] = 11;
  std::cout << "a1: " << a1 << std::endl;

  a1 = 12;
  std::cout << "my_array[0]: " << my_array[0] << std::endl;
}

int main(int argc, char* argv[])
{
  array_alias_example();
  transformation_example();
}
