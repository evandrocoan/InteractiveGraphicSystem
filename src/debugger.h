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


/**
 * Preprocessor directive designed to cause the current source file to be included only once in a
 * single compilation. Thus, serves the same purpose as #include guards, but with several
 * advantages, including: less code, avoidance of name clashes, and sometimes improvement in
 * compilation speed. In main file this is enabled by default.
 */
#pragma once

#include "utilities.h"



/**
 * This is to view internal program data while execution. Default value: 0
 *
 *  0  - Disables this feature.
 *  1  - Basic debugging.
 *  2  - Run the `doctest` Unit Tests.
 */
#define DEBUG_LEVEL 1



#define DEBUG_LEVEL_DISABLED_DEBUG 0
#define DEBUG_LEVEL_BASIC_DEBUG    1
#define DEBUG_LEVEL_RUN_UNIT_TESTS 2

/**
 * MemoryManager debugging.
 */
#if DEBUG_LEVEL > DEBUG_LEVEL_DISABLED_DEBUG

  /**
   * A value like a127 (111111) for 'g_debugLevel' enables all 'a' mask debugging levels. To enable all
   * debugging levels at once, use "a127 b127 c127" etc, supposing the level 64 is the highest to each
   * mask 'a', 'b', 'c', etc.
   *
   * Level A debugging:
   * a1   - Basic debug messages.
   * a2   - Function entrances
   *
   * Level B debugging:
   * b1   - Basic debug messages.
   */
  const char* const g_debugLevel = "a1 a2 b1";


  #define DEBUG
  #include <stdlib.h>
  #include <stdio.h>
  #include <cstring>
  #include <iostream>
  #include <string>
  #include <cstdarg>
  #include <chrono>
  #include <ctime>


  // C like printf support on C++
  #include "tinyformat.h"

  // C++ -> Utilities library -> Date and time utilities -> C-style date and time utilities -> std:clock
  // http://en.cppreference.com/w/cpp/chrono/c/clock
  //
  // Measure time in Linux - time vs clock vs getrusage vs clock_gettime vs gettimeofday vs timespec_get?
  // https://stackoverflow.com/questions/12392278/measure-time-in-linux-time-vs-clock-vs-getrusage-vs-clock-gettime-vs-gettimeof
  extern std::clock_t _debugger_current_saved_c_time;
  extern std::chrono::time_point< std::chrono::high_resolution_clock > _debugger_current_saved_chrono_time;


  /**
   * Print like function for logging putting a new line at the end of string. See the variables
   * 'g_debugLevel' for the available levels.
   *
   * On this function only, a time stamp as `7.484e+003 7.484e+003` will be used. It means the `CPU
   * time used`time in milliseconds and the `Wall clock time passed` respectively.
   *
   * @param level     the debugging desired level to be printed.
   * @param ...       variable number os formating arguments parameters.
   */
  #define LOG( level, ... ) \
  do \
  { \
    if( __computeDeggingLevel( #level ) ) \
    { \
      std::clock_t c_end = std::clock(); \
      auto t_end = std::chrono::high_resolution_clock::now(); \
      std::chrono::time_point< std::chrono::system_clock > now = std::chrono::system_clock::now();  \
      auto duration = now.time_since_epoch(); \
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
      struct tm *aTime = localtime(&theTime); \
      std::cout << tfm::format( "%02d:%02d:%02d:%03d:%03d %.3e %.3e %s/%s:%s ", \
              aTime->tm_hour, minutes.count(), seconds.count(), milliseconds.count(), microseconds.count(), /* nanoseconds.count(), */ \
              std::chrono::duration<double, std::milli>(t_end-_debugger_current_saved_chrono_time).count(), \
              (1000.0 * (c_end - _debugger_current_saved_c_time)) / CLOCKS_PER_SEC, \
              __FILE__, __FUNCTION__, __LINE__ \
          ) \
          << tfm::format( __VA_ARGS__ ) << std::endl; \
      _debugger_current_saved_c_time = c_end; \
      _debugger_current_saved_chrono_time = t_end; \
    } \
  } \
  while( 0 )

  /**
   * The same as LOGLN(...) just below, but do not put automatically a new line.
   */
  #define LOGLN( level, ... ) \
  do \
  { \
    if( __computeDeggingLevel( #level ) ) \
    { \
      std::cout << tfm::format( __VA_ARGS__ ); \
    } \
  } \
  while( 0 )

  /**
   * The same as LOGLN(...), but it is for standard program output.
   */
  #define PRINT( level, ... ) \
  do \
  { \
    if( __computeDeggingLevel( #level ) ) \
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
    if( __computeDeggingLevel( #level ) ) \
    { \
      std::cout << tfm::format( __VA_ARGS__ ); \
    } \
  } \
  while( 0 )


  /**
   * Determines whether the given debug level is enabled.
   *
   * @param debugLevel       the given char* string level to the debugger.
   * @return true when the current debug output is enabled, false otherwise.
   */
  inline bool __computeDeggingLevel( const char* debugLevel )
  {
  #define COMPUTE_DEBUGGING_LEVEL_DEBUG      0
  #define COMPUTE_DEBUGGING_DEBUG_INPUT_SIZE 32

    int inputLevel;
    int builtInLevel;

    int inputLevelSize;
    int builtInLevelSize;

    int inputLevelTokenSize;
    int builtInLevelTokenSize;

    char* inputLevelToken;
    char* builtInLevelToken;

    char builtInLevelChar[ COMPUTE_DEBUGGING_DEBUG_INPUT_SIZE ];
    char inputLevelChar  [ COMPUTE_DEBUGGING_DEBUG_INPUT_SIZE ];
    char inputLevelChars [ COMPUTE_DEBUGGING_DEBUG_INPUT_SIZE ][ COMPUTE_DEBUGGING_DEBUG_INPUT_SIZE ];

    int        inputLevels  = 0;
    const char separator[2] = " ";

    inputLevelSize   = strlen( debugLevel );
    builtInLevelSize = strlen( g_debugLevel );

    if( ( 2 > inputLevelSize && inputLevelSize > COMPUTE_DEBUGGING_DEBUG_INPUT_SIZE )
      || ( 2 > builtInLevelSize && builtInLevelSize > COMPUTE_DEBUGGING_DEBUG_INPUT_SIZE ) )
    {
      std::cout << "ERROR while processing the DEBUG LEVEL: " << debugLevel << std::endl;
      std::cout << "! The masks sizes are " << inputLevelSize << " and " << builtInLevelSize;
      std::cout << ", but they must to be between 1 and 32." << std::endl;

      exit( EXIT_FAILURE );
    }

    strcpy( inputLevelChar, debugLevel );
    strcpy( builtInLevelChar, g_debugLevel );

    // So, how do we debug the debugger?
  #if COMPUTE_DEBUGGING_LEVEL_DEBUG > 0
    int currentExternLoop = 0;
    int currentInternLoop = 0;

    std::cout << "\ndebugLevel: " << debugLevel << ", inputLevelSize: " << inputLevelSize << std::endl;
    std::cout << "g_debugLevel: " << g_debugLevel << ", builtInLevelSize: " << builtInLevelSize << std::endl;
  #endif

    inputLevelToken = strtok( inputLevelChar, separator );

    do
    {
      strcpy( inputLevelChars[ inputLevels++ ], inputLevelToken );

    } while( ( inputLevelToken = strtok( NULL, separator ) ) != NULL );

    while( inputLevels-- > 0 )
    {
    #if COMPUTE_DEBUGGING_LEVEL_DEBUG > 0
      currentInternLoop = 0;
      std::cout << "CURRENT_ExternLoop: " << currentExternLoop++ << std::endl;
    #endif

      builtInLevelToken   = strtok( builtInLevelChar, separator );
      inputLevelTokenSize = strlen( inputLevelChars[ inputLevels ] );

      do
      {
        builtInLevelTokenSize = strlen( builtInLevelToken );

      #if COMPUTE_DEBUGGING_LEVEL_DEBUG > 0
        std::cout << "space" << std::endl;
        std::cout << "CURRENT_InternLoop: " << currentInternLoop++ << std::endl;

        std::cout << "builtInLevelToken: " << builtInLevelToken << std::endl;
        std::cout << "builtInLevelTokenSize: " << builtInLevelTokenSize << std::endl;
        std::cout << "inputLevelChars[" << inputLevels << "]: " << inputLevelChars[ inputLevels ] << std::endl;
        std::cout << "inputLevelTokenSize: " << inputLevelTokenSize << std::endl;
      #endif

        if( inputLevelTokenSize > 0
          && builtInLevelTokenSize > 0 )
        {
          if( isdigit( inputLevelChars[ inputLevels ][ 1 ] )
            && isdigit( builtInLevelToken[ 1 ] ) )
          {
            if( builtInLevelToken[ 0 ] == inputLevelChars[ inputLevels ][ 0 ] )
            {
              sscanf( &inputLevelChars[ inputLevels ][ 1 ], "%d", &inputLevel );
              sscanf( &builtInLevelToken[ 1 ], "%d", &builtInLevel );

            #if COMPUTE_DEBUGGING_LEVEL_DEBUG > 0
              std::cout << "builtInLevel: " << builtInLevel << std::endl;
              std::cout << "inputLevel: " << inputLevel << std::endl;
              std::cout << "Is activeated? " << ( ( inputLevel & builtInLevel ) > 0 ) << std::endl;
            #endif

              if( ( inputLevel & builtInLevel ) > 0 )
              {
                #if COMPUTE_DEBUGGING_LEVEL_DEBUG > 0
                  std::cout << "returning true..." << std::endl;
                #endif
                return true;
              }
            }
          }
        }

      } while( ( builtInLevelToken = strtok( NULL, separator ) ) != NULL );

    #if COMPUTE_DEBUGGING_LEVEL_DEBUG > 0
      std::cout << "space" << std::endl;
    #endif
    }

    #if COMPUTE_DEBUGGING_LEVEL_DEBUG > 0
      std::cout << "returning false..." << std::endl;
    #endif
    return false;
  }

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





