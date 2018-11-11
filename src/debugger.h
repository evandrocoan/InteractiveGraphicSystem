/*********************** Licensing *******************************************************
*
*   Copyright 2017 @ Evandro Coan, https://github.com/evandrocoan
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
#ifndef GTKMM_APP_DEBUGGER_INT_DEBUG_LEVEL_H
#define GTKMM_APP_DEBUGGER_INT_DEBUG_LEVEL_H

#include "utilities.h"

// C like printf support on C++
// https://github.com/c42f/tinyformat
#include "tinyformat.h"


/**
 * This is to view internal program data while execution. Default value: 0
 *
 *  0  - Disables this feature.
 *  1  - Basic debugging with time stamp.
 *  2  - Basic debugging without time stamp.
 */
#ifndef DEBUG_LEVEL
  #define DEBUG_LEVEL 1
#endif


#define DEBUG_LEVEL_DISABLED_DEBUG     0
#define DEBUG_LEVEL_WITH_TIME_STAMP    1
#define DEBUG_LEVEL_WITHOUT_TIME_STAMP 2

/**
 * Control all program debugging.
 */
#if DEBUG_LEVEL > DEBUG_LEVEL_DISABLED_DEBUG

  /**
   * A value like 127 binary(111111) enables all masked debugging levels.
   *
   * 1  - Error or very important messages.
   * 2  - Function entrances by `...`
   * 4  - Comment messages inside functions calls
   * 8  - High called functions, i.e., create very big massive text output
   * 16 - Window movement, zoom, etc.
   */
  extern int _debugger_int_debug_level;

  #define DEBUG
  #include <iostream>
  #include <chrono>
  #include <ctime>

  // C++ -> Utilities library -> Date and time utilities -> C-style date and time utilities -> std:clock
  // http://en.cppreference.com/w/cpp/chrono/c/clock
  //
  // Measure time in Linux - time vs clock vs getrusage vs clock_gettime vs gettimeofday vs timespec_get?
  // https://stackoverflow.com/questions/12392278/measure-time-in-linux-time-vs-clock-vs-getrusage-vs-clock-gettime-vs-gettimeof
  extern std::clock_t _debugger_current_saved_c_time;
  extern std::chrono::time_point< std::chrono::high_resolution_clock > _debugger_current_saved_chrono_time;

  #if DEBUG_LEVEL & DEBUG_LEVEL_WITH_TIME_STAMP
    #define _DEBUGGER_TIME_STAMP_HEADER \
      auto duration = chrono_clock_now.time_since_epoch(); \
      /* typedef std::chrono::duration< int, std::ratio_multiply< std::chrono::hours::period, std::ratio< 21 > >::type > Days; */ \
      /* Days days = std::chrono::duration_cast< Days >( duration ); */ \
      /* duration -= days; */ \
      auto hours = std::chrono::duration_cast< std::chrono::hours >( duration ); \
      duration -= hours; \
      auto minutes = std::chrono::duration_cast< std::chrono::minutes >( duration ); \
      duration -= minutes; \
      auto seconds = std::chrono::duration_cast< std::chrono::seconds >( duration ); \
      duration -= seconds; \
      auto milliseconds = std::chrono::duration_cast< std::chrono::milliseconds >( duration ); \
      duration -= milliseconds; \
      auto microseconds = std::chrono::duration_cast< std::chrono::microseconds >( duration ); \
      /* duration -= microseconds; */ \
      /* auto nanoseconds = std::chrono::duration_cast< std::chrono::nanoseconds >( duration ); */ \
      time_t theTime = time(NULL); \
      struct tm* aTime = localtime(&theTime); \
      std::cout << tfm::format( "%02d:%02d:%02d:%03d:%03d %.3e %.3e ", \
          aTime->tm_hour, minutes.count(), seconds.count(), milliseconds.count(), microseconds.count(), /* nanoseconds.count(), */ \
          std::chrono::duration<double, std::milli>(chrono_clock_now-_debugger_current_saved_chrono_time).count(), \
          (1000.0 * (ctime_clock_now - _debugger_current_saved_c_time)) / CLOCKS_PER_SEC \
      );
  #else
    #define _DEBUGGER_TIME_STAMP_HEADER
  #endif


  /**
   * Print like function for logging putting a new line at the end of string. See the variables
   * '_debugger_int_debug_level' for the available levels.
   *
   * On this function only, a time stamp on scientific notation as `d.dde+ddd d.ddde+ddd` will be
   * used. These values mean the `CPU time used` in milliseconds and the `Wall clock time passed`
   * respectively.
   *
   * @param level     the debugging desired level to be printed.
   * @param ...       variable number os formating arguments parameters.
   */
  #define LOG( level, ... ) \
  do \
  { \
    if( level & _debugger_int_debug_level ) \
    { \
      std::clock_t ctime_clock_now = std::clock(); \
      auto chrono_clock_now = std::chrono::high_resolution_clock::now(); \
      _DEBUGGER_TIME_STAMP_HEADER \
      std::cout << tfm::format( "%s|%s:%s ", \
          __FILE__, __FUNCTION__, __LINE__ \
      ) \
      << tfm::format( __VA_ARGS__ ) << std::endl; \
      _debugger_current_saved_c_time = ctime_clock_now; \
      _debugger_current_saved_chrono_time = chrono_clock_now; \
    } \
  } \
  while( 0 )

  /**
   * The same as LOG(...) just above, but do not put automatically a new line.
   */
  #define LOGL( level, ... ) \
  do \
  { \
    if( level & _debugger_int_debug_level ) \
    { \
      std::clock_t ctime_clock_now = std::clock(); \
      auto chrono_clock_now = std::chrono::high_resolution_clock::now(); \
      _DEBUGGER_TIME_STAMP_HEADER \
      std::cout << tfm::format( "%s|%s:%s ", \
          __FILE__, __FUNCTION__, __LINE__ \
      ) \
      << tfm::format( __VA_ARGS__ ); \
      _debugger_current_saved_c_time = ctime_clock_now; \
      _debugger_current_saved_chrono_time = chrono_clock_now; \
    } \
  } \
  while( 0 )

  /**
   * The same as LOG(...) just above, but do not put automatically a new line, neither time stamp.
   */
  #define LOGLN( level, ... ) \
  do \
  { \
    if( level & _debugger_int_debug_level ) \
    { \
      std::cout << tfm::format( __VA_ARGS__ ) << std::flush; \
    } \
  } \
  while( 0 )

  /**
   * The same as LOG(...), but it is for standard program output.
   */
  #define PRINT( level, ... ) \
  do \
  { \
    if( level & _debugger_int_debug_level ) \
    { \
      std::cout << tfm::format( __VA_ARGS__ ) << std::endl; \
    } \
  } \
  while( 0 )

  /**
   * The same as LOG(...), but it is for standard program output.
   */
  #define PRINTLN( level, ... ) \
  do \
  { \
    if( level & _debugger_int_debug_level ) \
    { \
      std::cout << tfm::format( __VA_ARGS__ ) << std::flush; \
    } \
  } \
  while( 0 )


#else

  #define LOG( level, ... )
  #define LOGLN( level, ... )

  /**
   * The same as LOGLN(...), but it is for standard program output when the debugging is disabled.
   */
  #define PRINT( level, ... ) \
  do \
  { \
    std::cout << tfm::format( __VA_ARGS__ ) << std::endl; \
  } \
  while( 0 )

  /**
   * The same as LOG(...), but it is for standard program output when the debugging is disabled.
   */
  #define PRINTLN( level, ... ) \
  do \
  { \
    std::cout << tfm::format( __VA_ARGS__ ); \
  } \
  while( 0 )


#endif // #if DEBUG_LEVEL > DEBUG_LEVEL_DISABLED_DEBUG


#endif // GTKMM_APP_DEBUGGER_INT_DEBUG_LEVEL_H


