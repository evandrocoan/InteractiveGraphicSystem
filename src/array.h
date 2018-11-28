#ifndef GTKMM_APP_ARRAY_H
#define GTKMM_APP_ARRAY_H

#include <array>
#include <cassert>
#include <iostream>

#include "traits.h"
#include "stacktrace.h"

template<typename condition, typename Then, typename Else>
struct ARRAY_DEFAULT_IF_TYPE {
  typedef Else Result;
};

template<typename Then, typename Else>
struct ARRAY_DEFAULT_IF_TYPE<void, Then, Else> {
  typedef Then Result;
};

/**
 * C++ static polymorphism (CRTP) and using typedefs from derived classes
 * https://stackoverflow.com/questions/6006614/c-static-polymorphism-crtp-and-using-typedefs-from-derived-classes
 *
 * How to instantiate the base class when using the Curiously Recurring Template Pattern?
 * https://stackoverflow.com/questions/53463049/how-to-instantiate-the-base-class-when-using-the-curiously-recurring-template-pa
 */
template <unsigned int array_width, typename DataType, typename DerivedTypeDefault=void>
struct Array
{
  typedef typename ARRAY_DEFAULT_IF_TYPE<DerivedTypeDefault, Array, DerivedTypeDefault>::Result DerivedType;

  /**
   * Is it okay to inherit implementation from STL containers, rather than delegate?
   * https://stackoverflow.com/questions/2034916/is-it-okay-to-inherit-implementation-from-stl-containers-rather-than-delegate
   */
  std::array<DataType, array_width> _data;

  /**
   * std::array constructor inheritance
   * https://stackoverflow.com/questions/24280521/stdarray-constructor-inheritance
   */
  Array() {
  }

  Array(std::initializer_list< DataType > new_values) {
    unsigned int data_size = new_values.size();
    unsigned int column_index = 0;
    // std::cout << data_size << std::endl;

    if( data_size == 0 ) {
      #ifdef DEBUG
        std::cerr << "Welcome to the Ubuntu 16.04 awesome got nuts bug!\n";
        std::cerr << "Just give a look into his nonsense " << std::endl;
        print_stacktrace();
        std::cerr << "Array(new_values), " << "data_size: " << data_size << ", " << "array_width: " << array_width << std::endl;
      #endif
    }
    else if( data_size == 1 ) {
      this->clear(*(new_values.begin()));
    }
    else {
      // std::cerr << "Array(new_values), " << "data_size: " << data_size << ", " << "array_width: " << array_width << std::endl;
      // if(data_size != array_width) std::runtime_error( "" );
      assert(data_size == array_width);

      for( auto column : new_values ) {
        this->_data[column_index] = column;
        column_index++;
      }
    }
  }

  /**
   * Overloads the `[]` array access operator, allowing you to access this class objects as the
   * where usual `C` arrays.
   *
   * How to implement bound checking for std::array?
   * https://stackoverflow.com/questions/49419089/how-to-implement-bound-checking-for-stdarray
   *
   * @param  line the current line you want to access
   * @return      a pointer to the current line
   */
  DataType operator[](unsigned int line) && {
    assert(line < array_width);
    return this->_data[line];
  }

  DataType const& operator[](unsigned int line) const& {
    assert(line < array_width);
    return this->_data[line];
  }

  DataType& operator[](unsigned int line) & {
    assert(line < array_width);
    return this->_data[line];
  }

  /**
   * Generic Data to Object operators.
   */
  bool operator<=(const DataType& data) const {
    for( unsigned int index = 0; index < array_width; index++ ) {
      if( this->_data[index] > data ) {
        return false;
      }
    } return true;
  }

  bool operator<(const DataType& data) const {
    for( unsigned int index = 0; index < array_width; index++ ) {
      if( this->_data[index] >= data ) {
        return false;
      }
    } return true;
  }

  bool operator>=(const DataType& data) const {
    for( unsigned int index = 0; index < array_width; index++ ) {
      if( this->_data[index] < data ) {
        return false;
      }
    } return true;
  }

  bool operator>(const DataType& data) const {
    for( unsigned int index = 0; index < array_width; index++ ) {
      if( this->_data[index] <= data ) {
        return false;
      }
    } return true;
  }

  bool operator==(const DataType& data) const {
    for( unsigned int index = 0; index < array_width; index++ ) {
      if( this->_data[index] != data ) {
        return false;
      }
    } return true;
  }

  bool operator!=(const DataType& data) const {
    for( unsigned int index = 0; index < array_width; index++ ) {
      if( this->_data[index] == data ) {
        return false;
      }
    } return true;
  }

  DerivedType operator-() const {
    DerivedType new_array;
    for( unsigned int index = 0; index < array_width; index++ ) {
      new_array._data[index] = -_data[index];
    }
    return new_array;
  }

  DerivedType operator+(const big_double& data) {
    DerivedType new_array;
    for( unsigned int index = 0; index < array_width; index++ ) {
      new_array._data[index] = _data[index] + data;
    }
    return new_array;
  }

  DerivedType operator-(const big_double& data) {
    DerivedType new_array;
    for( unsigned int index = 0; index < array_width; index++ ) {
      new_array._data[index] = _data[index] - data;
    }
    return new_array;
  }

  DerivedType& operator+=(const big_double& data) {
    for( unsigned int index = 0; index < array_width; index++ ) {
      this->_data[index] += data;
    }
    return *static_cast<DerivedType*>(this);
  }

  DerivedType& operator-=(const big_double& data) {
    for( unsigned int index = 0; index < array_width; index++ ) {
      this->_data[index] -= data;
    }
    return *static_cast<DerivedType*>(this);
  }

  DerivedType operator/(const double& data) {
    unsigned int column;
    DerivedType new_array;

    for(column = 0; column < array_width; column++) {
      new_array._data[column] = _data[column] / data;
    }
    return new_array;
  }

  DerivedType divide(const double& data) {
    DerivedType result = this->operator/(data);
    _data = result._data;
    return result;
  }

  /**
   * Object to Object operators.
   */
  bool operator<=(const Array& object) const {
    for( unsigned int index = 0; index < array_width; index++ ) {
      if( this->_data[index] > object._data[index] ) {
        return false;
      }
    } return true;
  }

  bool operator<(const Array& object) const {
    for( unsigned int index = 0; index < array_width; index++ ) {
      if( this->_data[index] >= object._data[index] ) {
        return false;
      }
    } return true;
  }

  bool operator>=(const Array& object) const {
    for( unsigned int index = 0; index < array_width; index++ ) {
      if( this->_data[index] < object._data[index] ) {
        return false;
      }
    } return true;
  }

  bool operator>(const Array& object) const {
    for( unsigned int index = 0; index < array_width; index++ ) {
      if( this->_data[index] <= object._data[index] ) {
        return false;
      }
    } return true;
  }

  bool operator==(const Array& object) const {
    for( unsigned int index = 0; index < array_width; index++ ) {
      if( this->_data[index] != object._data[index] ) {
        return false;
      }
    } return true;
  }

  bool operator!=(const Array& object) const {
    for( unsigned int index = 0; index < array_width; index++ ) {
      if( this->_data[index] == object._data[index] ) {
        return false;
      }
    } return true;
  }

  template<typename BaseClass>
  DerivedType operator+(const Array< array_width, DataType, BaseClass >& array) {
    unsigned int column;
    DerivedType new_array;

    for(column = 0; column < array_width; column++) {
      new_array._data[column] = _data[column] + array._data[column];
    }
    return new_array;
  }

  template<typename BaseClass>
  DerivedType operator-(const Array< array_width, DataType, BaseClass >& array) {
    unsigned int column;
    DerivedType new_array;

    for(column = 0; column < array_width; column++) {
      new_array._data[column] = _data[column] - array._data[column];
    }
    return new_array;
  }

  template<typename BaseClass>
  DerivedType& operator+=(const Array< array_width, DataType, BaseClass >& array) {
    unsigned int column;

    for(column = 0; column < array_width; column++) {
      _data[column] += array._data[column];
    }
    return *static_cast<DerivedType*>(this);
  }

  template<typename BaseClass>
  DerivedType& operator-=(const Array< array_width, DataType, BaseClass >& array) {
    unsigned int column;

    for(column = 0; column < array_width; column++) {
      _data[column] -= array._data[column];
    }
    return *static_cast<DerivedType*>(this);
  }

  template<typename BaseClass>
  DerivedType operator*(const Array< array_width, DataType, BaseClass >& array) {
    unsigned int column;
    DerivedType new_array;

    for(column = 0; column < array_width; column++) {
      new_array._data[column] = _data[column] * array._data[column];
    }
    return new_array;
  }

  template<typename BaseClass>
  DerivedType& multiply(const Array< array_width, DataType, BaseClass >& array) {
    _data = this->operator*(array)._data;
    return *static_cast<DerivedType*>(this);
  }

  /**
   * The Array<> type includes the Matrix<> type, because you can multiply a `Array` by an `Matrix`,
   * but not a vice-versa.
   */
  template<typename BaseClass>
  DerivedType& multiply(const Array
      <
          array_width,
          Array< array_width, DataType, BaseClass >,
          Array< array_width, DataType, BaseClass >
      > matrix)
  {
    unsigned int column;
    unsigned int step;
    DataType old_array[array_width];

    for(column = 0; column < array_width; column++)
    {
      old_array  [column] = this->_data[column];
      this->_data[column] = 0;
    }

    for(column = 0; column < array_width; column++)
    {
      for(step = 0; step < array_width; step++)
      {
        this->_data[column] += old_array[step] * matrix._data[step][column];
      }
    }
    return *static_cast<DerivedType*>(this);
  }

  /**
   * Set all the values on the array to the specified single data parameter.
   *
   * @param `initial` the value to the used
   */
  void clear(const DataType initial = 0) {
    unsigned int column_index = 0;

    for( ; column_index < array_width; column_index++ ) {
      this->_data[column_index] = initial;
    }
  }

  /**
   * Prints a more beauty version of the array when called on `std::cout << array << std::end;`
   */
  friend std::ostream& operator<<( std::ostream &output, const Array &array ) {
    unsigned int column;
    output << "(";

    for( column=0; column < array_width; column++ ) {
      output << array._data[column];

      if( column != array_width-1 ) {
        output << ", ";
      }
    }

    output << ")";
    return output;
  }
};

#endif // GTKMM_APP_ARRAY_H
