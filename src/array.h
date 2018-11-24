#ifndef GTKMM_APP_ARRAY_H
#define GTKMM_APP_ARRAY_H

#include <array>
#include <cassert>
#include <iostream>

#include "traits.h"
#include "stacktrace.h"

/**
 * C++ static polymorphism (CRTP) and using typedefs from derived classes
 * https://stackoverflow.com/questions/6006614/c-static-polymorphism-crtp-and-using-typedefs-from-derived-classes
 */
template <unsigned int array_width, typename DataType, typename DerivedType>
struct ArrayBase
{
  /**
   * Is it okay to inherit implementation from STL containers, rather than delegate?
   * https://stackoverflow.com/questions/2034916/is-it-okay-to-inherit-implementation-from-stl-containers-rather-than-delegate
   */
  std::array<DataType, array_width> _data;

  /**
   * std::array constructor inheritance
   * https://stackoverflow.com/questions/24280521/stdarray-constructor-inheritance
   */
  ArrayBase()
  {
  }

  ArrayBase(std::initializer_list< DataType > new_values)
  {
    unsigned int data_size = new_values.size();
    unsigned int column_index = 0;
    // std::cout << data_size << std::endl;

    if( data_size == 0 )
    {
      #ifdef DEBUG
        std::cerr << "Welcome to the Moodle VPL(Virtual Programming Lab) awesome got nuts bug!\n";
        std::cerr << "Just give a look into his nonsense " << std::endl;
        print_stacktrace();
        std::cerr << "Array(new_values), " << "data_size: " << data_size << ", " << "array_width: " << array_width << std::endl;
      #endif
    }
    else if( data_size == 1 )
    {
      this->clear(*(new_values.begin()));
    }
    else
    {
      // std::cerr << "Array(new_values), " << "data_size: " << data_size << ", " << "array_width: " << array_width << std::endl;
      // if(data_size != array_width) std::runtime_error( "" );
      assert(data_size == array_width);

      for( auto column : new_values )
      {
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
  DataType operator[](unsigned int line) &&
  {
    assert(line < array_width);
    return this->_data[line];
  }

  DataType const& operator[](unsigned int line) const&
  {
    assert(line < array_width);
    return this->_data[line];
  }

  DataType& operator[](unsigned int line) &
  {
    assert(line < array_width);
    return this->_data[line];
  }

  /**
   * Generic Data to Object operators.
   */
  bool operator<=(const DataType& data) const { for( unsigned int index = 0; index < array_width; index++ )
      { if( this->_data[index] > data ) { return false; } } return true; }

  bool operator<(const DataType& data) const { for( unsigned int index = 0; index < array_width; index++ )
      { if( this->_data[index] >= data ) { return false; } } return true; }

  bool operator>=(const DataType& data) const { for( unsigned int index = 0; index < array_width; index++ )
      { if( this->_data[index] < data ) { return false; } } return true; }

  bool operator>(const DataType& data) const { for( unsigned int index = 0; index < array_width; index++ )
      { if( this->_data[index] <= data ) { return false; } } return true; }

  bool operator==(const DataType& data) const { for( unsigned int index = 0; index < array_width; index++ )
      { if( this->_data[index] != data ) { return false; } } return true; }

  bool operator!=(const DataType& data) const { for( unsigned int index = 0; index < array_width; index++ )
      { if( this->_data[index] == data ) { return false; } } return true; }

  /**
   * Generic Object to Object operators.
   */
  bool operator<=(const ArrayBase& object) const { for( unsigned int index = 0; index < array_width; index++ )
      { if( this->_data[index] > object._data[index] ) { return false; } } return true; }

  bool operator<(const ArrayBase& object) const { for( unsigned int index = 0; index < array_width; index++ )
      { if( this->_data[index] >= object._data[index] ) { return false; } } return true; }

  bool operator>=(const ArrayBase& object) const { for( unsigned int index = 0; index < array_width; index++ )
      { if( this->_data[index] < object._data[index] ) { return false; } } return true; }

  bool operator>(const ArrayBase& object) const { for( unsigned int index = 0; index < array_width; index++ )
      { if( this->_data[index] <= object._data[index] ) { return false; } } return true; }

  bool operator==(const ArrayBase& object) const { for( unsigned int index = 0; index < array_width; index++ )
      { if( this->_data[index] != object._data[index] ) { return false; } } return true; }

  bool operator!=(const ArrayBase& object) const { for( unsigned int index = 0; index < array_width; index++ )
      { if( this->_data[index] == object._data[index] ) { return false; } } return true; }

  /**
   * Set all the values on the array to the specified single data parameter.
   *
   * @param `initial` the value to the used
   */
  void clear(const DataType initial = 0)
  {
    unsigned int column_index = 0;

    for( ; column_index < array_width; column_index++ )
    {
      this->_data[column_index] = initial;
    }
  }

  template<typename BaseClass>
  ArrayBase operator*(const ArrayBase< array_width, DataType, BaseClass >& array)
  {
    unsigned int column;
    ArrayBase new_array;

    for(column = 0; column < array_width; column++) {
      new_array._data[column] = _data[column] * array._data[column];
    }
    return new_array;
  }

  template<typename BaseClass>
  void multiply(const ArrayBase< array_width, DataType, BaseClass >& array)
  {
    _data = this->operator*(array)._data;
  }

  /**
   * The ArrayBase<> type includes the Matrix<> type, because you can multiply a `ArrayBase` by an `Matrix`,
   * but not a vice-versa.
   */
  template<typename BaseClass>
  void multiply(const ArrayBase
      <
          array_width,
          ArrayBase< array_width, DataType, BaseClass >,
          ArrayBase< array_width, DataType, BaseClass >
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
    // If you would like to preserve the original value, it can be returned here
    // return DerivedType{}._data = old_array;
  }

  /**
   * Prints a more beauty version of the array when called on `std::cout << array << std::end;`
   */
  friend std::ostream& operator<<( std::ostream &output, const ArrayBase &array )
  {
    unsigned int column;
    output << "(";

    for( column=0; column < array_width; column++ )
    {
      output << array._data[column];

      if( column != array_width-1 )
      {
        output << ", ";
      }
    }

    output << ")";
    return output;
  }
};

/**
 * Defines a simple array interface when the `Curiously Recurring Template Pattern` is not required.
 */
struct ConcreteArray{};

template <unsigned int array_width, typename DataType>
struct Array : public ArrayBase< array_width, DataType, ConcreteArray >
{
  using ArrayBase< array_width, DataType, ConcreteArray >::ArrayBase;
};

#endif // GTKMM_APP_ARRAY_H
