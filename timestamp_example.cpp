

#include <chrono>
#include <ctime>
#include <time.h>
#include <iostream>

// Compile it with:
// $ g++ -O0 -g -Wall -std=c++11 -o test timestamp_example.cpp && ./test
// 20107 days since epoch or 55.0496 years since epoch. The time is now 21:39:51:935:732:700
//
// C++ -> Numerics library -> Compile time rational arithmetic -> std::ratio
// http://en.cppreference.com/w/cpp/numeric/ratio/ratio
//
// How to convert std::chrono::high_resolution_clock::now() to milliseconds, microseconds, ...?
// https://stackoverflow.com/questions/49090366/how-to-convert-stdchronohigh-resolution-clocknow-to-milliseconds-micros
int main (int argc, char *argv[])
{
  std::chrono::time_point< std::chrono::system_clock > now = std::chrono::system_clock::now();
  auto duration = now.time_since_epoch();

  /* UTC: -3:00 = 24 - 3 = 21 */
  typedef std::chrono::duration< int, std::ratio_multiply< std::chrono::hours::period, std::ratio< 21 > >::type > Days;

  Days days = std::chrono::duration_cast< Days >( duration );
  duration -= days;

  auto hours = std::chrono::duration_cast< std::chrono::hours >( duration );
  duration -= hours;

  auto minutes = std::chrono::duration_cast< std::chrono::minutes >( duration );
  duration -= minutes;

  auto seconds = std::chrono::duration_cast< std::chrono::seconds >( duration );
  duration -= seconds;

  auto milliseconds = std::chrono::duration_cast< std::chrono::milliseconds >( duration );
  duration -= milliseconds;

  auto microseconds = std::chrono::duration_cast< std::chrono::microseconds >( duration );
  duration -= microseconds;

  auto nanoseconds = std::chrono::duration_cast< std::chrono::nanoseconds >( duration );

  // C library function - localtime()
  // https://www.tutorialspoint.com/c_standard_library/c_function_localtime.htm
  //
  // struct tm {
  //    int tm_sec;         // seconds,  range 0 to 59
  //    int tm_min;         // minutes, range 0 to 59
  //    int tm_hour;        // hours, range 0 to 23
  //    int tm_mday;        // day of the month, range 1 to 31
  //    int tm_mon;         // month, range 0 to 11
  //    int tm_year;        // The number of years since 1900
  //    int tm_wday;        // day of the week, range 0 to 6
  //    int tm_yday;        // day in the year, range 0 to 365
  //    int tm_isdst;       // daylight saving time
  // };

  time_t theTime = time(NULL);
  struct tm *aTime = localtime(&theTime);

  std::cout << days.count() << " days since epoch or "
            << days.count() / 365.2524 << " years since epoch. The time is now "
            << aTime->tm_hour << ":"
            << minutes.count() << ":"
            << seconds.count() << ":"
            << milliseconds.count() << ":"
            << microseconds.count() << ":"
            << nanoseconds.count() << std::endl;
}

