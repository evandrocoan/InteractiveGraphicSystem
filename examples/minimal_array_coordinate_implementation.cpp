#include <array>
#include <iostream>

template<typename condition, typename Then, typename Else>
struct ARRAY_DEFAULT_IF_TYPE {
  typedef Else Result;
};

template<typename Then, typename Else>
struct ARRAY_DEFAULT_IF_TYPE<void, Then, Else> {
  typedef Then Result;
};

// How to instantiate the base class when using the Curiously Recurring Template Pattern?
// https://stackoverflow.com/questions/53463049/how-to-instantiate-the-base-class-when-using-the-curiously-recurring-template-pa
template <unsigned int array_width, typename DataType, typename DerivedTypeDefault=void>
struct Array {
  std::array<DataType, array_width> _data;

  typedef typename ARRAY_DEFAULT_IF_TYPE
      <
        DerivedTypeDefault,
        Array,
        DerivedTypeDefault
      >
      ::Result DerivedType;

  Array() {
    for(int index = 0; index < array_width; ++index) _data[index] = 1;
  }

  DerivedType operator/(const double& data) {
    unsigned int column;
    DerivedType new_array;

    for(column = 0; column < array_width; column++) {
      new_array._data[column] = _data[column] / data;
    }
    return new_array;
  }

  friend std::ostream& operator<<( std::ostream &output, const Array &array ) {
    unsigned int column; output << "(";
    for( column=0; column < array_width; column++ ) {
      output << array._data[column];
      if( column != array_width-1 ) {
        output << ", ";
      }
    }
    output << ")"; return output;
  }
};

struct Coordinate : public Array<3, double, Coordinate> {
  typedef Array< 3, double, Coordinate > SuperClass;
  double& x;
  double& y;
  double& z;

  Coordinate() : SuperClass{}, x{this->_data[0]}, y{this->_data[1]}, z{this->_data[2]} {}
};

int main() {
  Coordinate coordinate;
  std::cout << "coordinate: " << coordinate << std::endl;

  Coordinate new_coordinate = coordinate / 10.0;
  std::cout << "new_coordinate: " << new_coordinate << std::endl;

  Array<5, int> int_array;
  std::cout << "int_array: " << int_array << std::endl;

  Array<5, int> new_int_array = int_array / 10;
  std::cout << "new_int_array: " << new_int_array << std::endl;
}

