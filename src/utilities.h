/*********************** Licensing *******************************************************
*
*   Copyright 2017 @ Evandro Coan
*
*   Program Main Page: https://github.com/evandrocoan/ObjectBeautifier
*
*  This program is free software; you can redistribute it and/or modify it
*  under the terms of the GNU General Public License as published by the
*  Free Software Foundation; either version 3 of the License, or ( at
*  your option ) any later version.
*
*  This program is distributed in the hope that it will be useful, but
*  WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
*  General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
*****************************************************************************************
*/

/* Deprecated the usage of `#pragma once` due:

> #pragma once does have one drawback (other than being non-standard) and that is if you have the
> same file in different locations (we have this because our build system copies files around) then
> the compiler will think these are different files.
>
> Is #pragma once a safe include guard?
> https://stackoverflow.com/a/1946730/4934640
*/
#ifndef GTKMM_APP_DEBUGGER_INT_UTILITIES_H
#define GTKMM_APP_DEBUGGER_INT_UTILITIES_H

#include <string>
#include <cstdarg>

// M_PI flagged as undeclared identifier
// https://stackoverflow.com/questions/26065359/m-pi-flagged-as-undeclared-identifier
#define _USE_MATH_DEFINES

#include <cmath>
#include <vector>

#ifndef M_PI
  #define M_PI 3.14159265358979323846
#endif

#include "traits.h"

inline big_double convert_degrees_to_radians(big_double degrees)
{
  return M_PI * (std::fmod(degrees, 360.0) / 180.0);
}

inline big_double factorial(int n)
{
    big_double x = 1.0;
    for (int index = 1; index <= n; index++)
      x *= (big_double)index;
    return x;
}

template<typename Type>
void normalize_angle(Type& angle, const Type value) {
  angle += value;
  while (angle >= 360) {
    angle -= 360;
  }
  while (angle < 0) {
    angle += 360;
  }
}

/**
 *  Calculates a static array size.
 */
#if !defined STATIC_ARRAY_SIZE
  #define STATIC_ARRAY_SIZE( array ) ( sizeof( ( array ) ) / sizeof( ( array )[0] ) )
#endif

/**
 * Missing string printf. This is safe and convenient but not exactly efficient.
 *
 * @param fmt     a char array
 * @param ...     a variable length number of formating characters.
 *
 * @see http://stackoverflow.com/a/10150393/4934640
 * @see http://stackoverflow.com/questions/2342162/stdstring-formatting-like-sprintf/10150393#10150393
 */
inline std::string format(const char* fmt, ...)
{
  int   size   = 512;
  char* buffer = new char[size];

  va_list var_args;
  va_start(var_args, fmt);

  int nsize = vsnprintf(buffer, size, fmt, var_args);

  //fail delete buffer and try again
  if(size<=nsize)
  {
    delete[] buffer;

    buffer = 0;
    buffer = new char[nsize+1]; //+1 for /0
    nsize  = vsnprintf(buffer, size, fmt, var_args);
  }

  std::string ret(buffer);
  va_end(var_args);

  delete[] buffer;
  return ret;
}

#endif // GTKMM_APP_DEBUGGER_INT_UTILITIES_H

