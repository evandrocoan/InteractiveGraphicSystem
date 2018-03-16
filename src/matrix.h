#ifndef GTKMM_APP_COORDINATE
#define GTKMM_APP_COORDINATE

#include <iostream>

/**
 * C++ Matrix Class
 * https://stackoverflow.com/questions/2076624/c-matrix-class
 */
template <int width, int height>
struct Matrix
{
  long int _data[height][width];

  friend std::ostream &operator<<( std::ostream &output, const Matrix &matrix )
  {
    int i, j;

    for( i=0; i < height; i++ )
    {
      for( j=0; j < width; j++ )
      {
        output << matrix._data[i][j] << ", ";
      }

      output << matrix._data[i][j] << "\n";
    }
    return output;
  }
};

#endif // GTKMM_APP_Coordinate
