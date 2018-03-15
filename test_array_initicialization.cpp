#include <iostream>


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

int main (int argc, char *argv[])
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

